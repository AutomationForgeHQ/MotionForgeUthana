// Uthana's settings on a Motion Definition, named as Uthana's API names them.

#pragma once

#include "CoreMinimal.h"
#include "MotionPipeline.h"
#include "UthanaPipeline.generated.h"

/**
 * What Uthana is asked to do with a definition's prompt. Every property is sent under the name shown.
 */
UCLASS(meta = (DisplayName = "Uthana"))
class MOTIONFORGEUTHANA_API UUthanaPipeline : public UMotionPipeline
{
	GENERATED_BODY()

public:

	UUthanaPipeline();

	/**
	 * The model. text-to-motion-3.0 makes 4 to 10 seconds, animates hands well, and takes no seed: a
	 * take that is not kept cannot be made again. It is the only model this plugin sends to.
	 */
	UPROPERTY(EditAnywhere, Category = "Uthana", meta = (DisplayName = "model", WireName = "model", GetOptions = "GetModelOptions"))
	FString Model;

	/**
	 * Let Uthana rewrite the prompt into its own phrasing before generating. It helps a terse prompt and
	 * can flatten deliberate wording such as tempo words, so try both on a prompt that matters.
	 */
	UPROPERTY(EditAnywhere, Category = "Uthana", meta = (DisplayName = "rewrite_prompt", WireName = "rewrite_prompt"))
	bool bRewritePrompt = true;

	UFUNCTION()
	static TArray<FString> GetModelOptions();

	// UMotionPipeline
	virtual FName GetProviderId() const override;
	virtual FString GetModelId() const override { return Model; }
	virtual void Apply(FMotionSubmitRequest& Request) const override;
	virtual void Validate(const FMotionSubmitRequest& Request, TArray<FString>& OutProblems, TArray<FString>& OutWarnings) const override;
	virtual TArray<FString> DescribeSent(const FMotionSubmitRequest& Request) const override;
	virtual void MigrateLegacy(const FString& LegacyModelId, bool bLegacyRewritePrompt, const FMotionControl& LegacyControl) override;
};
