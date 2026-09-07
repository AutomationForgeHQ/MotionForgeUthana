#include "MotionForgeUthana.h"

#include "UthanaProvider.h"
#include "MotionForge.h"

DEFINE_LOG_CATEGORY(LogMotionForgeUthana);

void FMotionForgeUthanaModule::StartupModule()
{
	// GetPtr loads MotionForge rather than looking it up. A .uplugin dependency guarantees MotionForge
	// is enabled, not that its module started first, so a lookup here works on some runs and returns
	// null on others - and the failure mode is a provider that silently never appears.
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
