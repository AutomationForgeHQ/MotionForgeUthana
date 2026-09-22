// Project Settings > Automation Forge > MotionForge Uthana.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UthanaSettings.generated.h"

/** Which Uthana plan this project's account is on. Nothing can detect it; only the account knows. */
UENUM(BlueprintType)
enum class EUthanaPlan : uint8
{
	/** Every take bills its requested seconds when submitted, kept or not. Downloads are free. */
	PayAsYouGo		UMETA(DisplayName = "pay as you go"),

	/** Generating is free; downloading a take uses its seconds of the monthly quota. */
	Subscription	UMETA(DisplayName = "subscription")
};

// Moved here from the core MotionForge settings, where Uthana's plan and price were applied to every
// provider that billed - which priced a rented Kimodo GPU per generated second. The core now asks each
// provider how it bills, and this page is how Uthana answers.

/**
 * How your Uthana account is billed, so every cost line and confirmation in MotionForge states the
 * right price before you spend. Set the plan to match your account; the two plans reverse the right
 * way to work.
 */
UCLASS(config = Editor, defaultconfig, meta = (DisplayName = "MotionForge Uthana"))
class MOTIONFORGEUTHANA_API UUthanaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	virtual FName GetContainerName() const override { return TEXT("Project"); }
	virtual FName GetCategoryName() const override { return TEXT("Automation Forge"); }

	static const UUthanaSettings* Get() { return GetDefault<UUthanaSettings>(); }

	/**
	 * Your plan. Pay as you go: ask for few takes, since each one bills. Subscription: ask for many and
	 * import only the keeper, since importing is what uses the quota.
	 */
	UPROPERTY(config, EditAnywhere, Category = "Billing", meta = (DisplayName = "plan"))
	EUthanaPlan Plan = EUthanaPlan::PayAsYouGo;

	/** Uthana's published pay-as-you-go price for text-to-motion-3.0, in USD per generated second. */
	static constexpr float PublishedRatePerGeneratedSecond = 0.10f;

	/**
	 * What one generated second costs on pay as you go. Starts at Uthana's published price for
	 * text-to-motion-3.0, 0.10 USD, and every cost line says so until you change it.
	 */
	UPROPERTY(config, EditAnywhere, Category = "Billing", meta = (ClampMin = 0.0, EditCondition = "Plan == EUthanaPlan::PayAsYouGo"))
	float RatePerGeneratedSecond = PublishedRatePerGeneratedSecond;

	/** The rate is still Uthana's published price rather than one entered here. */
	bool IsPublishedRate() const
	{
		return Currency == TEXT("USD") && FMath::IsNearlyEqual(RatePerGeneratedSecond, PublishedRatePerGeneratedSecond);
	}

	/** What one downloaded second is worth on a subscription, when you want quota shown as money. Zero shows seconds only. */
	UPROPERTY(config, EditAnywhere, Category = "Billing", meta = (ClampMin = 0.0, EditCondition = "Plan == EUthanaPlan::Subscription"))
	float RatePerDownloadedSecond = 0.f;

	/** The currency the rates are in, as a code: USD, EUR, GBP, CHF. */
	UPROPERTY(config, EditAnywhere, Category = "Billing")
	FString Currency = TEXT("USD");
};
