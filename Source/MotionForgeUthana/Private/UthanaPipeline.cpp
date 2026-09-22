#include "UthanaPipeline.h"

#include "IMotionProvider.h"
#include "MotionForgeUthana.h"
#include "UthanaProvider.h"

UUthanaPipeline::UUthanaPipeline()
{
	Model = TEXT("text-to-motion-3.0");
}

FName UUthanaPipeline::GetProviderId() const
{
	return FUthanaProvider::ProviderId;
}

TArray<FString> UUthanaPipeline::GetModelOptions()
{
	// Only the asynchronous family is implemented, and 3.0 is the one worth generating with. 2.0 exists
	// in the API on a different, synchronous path, so offering it here would offer something that fails.
	return { TEXT("text-to-motion-3.0") };
}

void UUthanaPipeline::Apply(FMotionSubmitRequest& Request) const
{
	Request.ModelId = Model;
	Request.bRewritePrompt = bRewritePrompt;

	// Uthana reads none of these. Cleared so the record of what was sent says what was sent.
	Request.Control.Seed = -1;
	Request.Control.bSplitPromptIntoBeats = false;
	Request.Control.BeatSeconds.Reset();
}

void UUthanaPipeline::Validate(
	const FMotionSubmitRequest& Request, TArray<FString>& OutProblems, TArray<FString>& OutWarnings) const
{
	if (!GetModelOptions().Contains(Model))
	{
		OutProblems.Add(FString::Printf(TEXT("Uthana has no model called '%s' on this path. Use text-to-motion-3.0."), *Model));
	}

	if (Request.ProviderCharacterId.IsEmpty())
	{
		OutProblems.Add(TEXT("The character has not been uploaded to Uthana. Upload it from the Character card; it is free."));
	}
}

TArray<FString> UUthanaPipeline::DescribeSent(const FMotionSubmitRequest& Request) const
{
	return {
		FString::Printf(TEXT("model=%s"), *Model),
		FString::Printf(TEXT("rewrite_prompt=%s"), bRewritePrompt ? TEXT("true") : TEXT("false")),
		FString::Printf(TEXT("length=%d"), Request.Length),
	};
}

void UUthanaPipeline::MigrateLegacy(const FString& LegacyModelId, bool bLegacyRewritePrompt, const FMotionControl& LegacyControl)
{
	if (!LegacyModelId.IsEmpty())
	{
		if (GetModelOptions().Contains(LegacyModelId))
		{
			Model = LegacyModelId;
		}
		else
		{
			UE_LOG(LogMotionForgeUthana, Log,
				TEXT("Dropped the model '%s' while moving settings into the Uthana pipeline: this plugin "
					 "sends only to %s."), *LegacyModelId, *Model);
		}
	}

	bRewritePrompt = bLegacyRewritePrompt;
}
