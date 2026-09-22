#include "MotionForgeUthana.h"

#include "UthanaProvider.h"
#include "UthanaSettings.h"
#include "MotionForge.h"

#include "Misc/ConfigCacheIni.h"

namespace UthanaMigration
{
	/**
	 * Carry the plan and price over from where they used to live, once.
	 *
	 * They were on the core MotionForge page, applied to every provider that billed. A project that set
	 * a subscription there would otherwise be priced as pay as you go after the move - wrong in the
	 * direction that matters. Only when this page has never been set, so a deliberate choice here is
	 * never overwritten.
	 */
	static void CarryBillingOver()
	{
		const TCHAR* OldSection = TEXT("/Script/MotionForge.MotionForgeSettings");
		const TCHAR* NewSection = TEXT("/Script/MotionForgeUthana.UthanaSettings");

		FString OldModel;
		if (!GConfig || !GConfig->GetString(OldSection, TEXT("BillingModel"), OldModel, GEditorIni))
		{
			return;
		}

		FString Existing;
		if (GConfig->GetString(NewSection, TEXT("Plan"), Existing, GEditorIni))
		{
			return;
		}

		UUthanaSettings* Settings = GetMutableDefault<UUthanaSettings>();

		float Rate = 0.f;
		const bool bHasRate = GConfig->GetFloat(OldSection, TEXT("RatePerBilledSecond"), Rate, GEditorIni);

		FString Currency;
		if (GConfig->GetString(OldSection, TEXT("Currency"), Currency, GEditorIni) && !Currency.IsEmpty())
		{
			Settings->Currency = Currency;
		}

		if (OldModel.Contains(TEXT("Downloaded")))
		{
			Settings->Plan = EUthanaPlan::Subscription;
			if (bHasRate) { Settings->RatePerDownloadedSecond = Rate; }
		}
		else
		{
			Settings->Plan = EUthanaPlan::PayAsYouGo;
			if (bHasRate && Rate > 0.f) { Settings->RatePerGeneratedSecond = Rate; }
		}

		// A defaultconfig class writes to the project's Default ini through this, and SaveConfig would
		// write nowhere the next session reads.
		Settings->TryUpdateDefaultConfigFile();

		UE_LOG(LogMotionForgeUthana, Log,
			TEXT("Moved the Uthana plan (%s) from the MotionForge settings page to MotionForge Uthana's own."),
			Settings->Plan == EUthanaPlan::PayAsYouGo ? TEXT("pay as you go") : TEXT("subscription"));
	}
}

DEFINE_LOG_CATEGORY(LogMotionForgeUthana);

void FMotionForgeUthanaModule::StartupModule()
{
	// GetPtr loads MotionForge rather than looking it up. A .uplugin dependency guarantees MotionForge
	// is enabled, not that its module started first, so a lookup here works on some runs and returns
	// null on others - and the failure mode is a provider that silently never appears.
	UthanaMigration::CarryBillingOver();

	if (FMotionForgeModule* MotionForge = FMotionForgeModule::GetPtr())
	{
		Provider = MakeShared<FUthanaProvider>();
		MotionForge->RegisterProvider(Provider.ToSharedRef());
	}
	else
	{
		UE_LOG(LogMotionForgeUthana, Error,
			TEXT("MotionForgeUthana could not load the MotionForge module, so the Uthana provider is "
			     "not available. Check that the MotionForge plugin is enabled."));
	}
}

void FMotionForgeUthanaModule::ShutdownModule()
{
	// GetPtrIfLoaded, never GetPtr: loading a module during teardown to tell it something is being
	// torn down would be worse than doing nothing.
	if (FMotionForgeModule* MotionForge = FMotionForgeModule::GetPtrIfLoaded())
	{
		MotionForge->UnregisterProvider(FUthanaProvider::ProviderId);
	}

	Provider.Reset();
}

IMPLEMENT_MODULE(FMotionForgeUthanaModule, MotionForgeUthana)
