// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "TouristPreferencesFragment.generated.h"

/**
 * Fragment that stores the tourist's mostly-static preference profile.
 * This fragment should be initialized via a Trait at spawn time.
 */
USTRUCT(BlueprintType)
struct FTouristPreferencesFragment final : public FMassFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Preferences")
	float WBench = 0.33f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Preferences")
	float WMonument = 0.33f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Preferences")
	float WFastFood = 0.34f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Preferences", meta=(ClampMin="0.0", ClampMax="1.0"))
	float PriceSensitivity = 0.5f;

public:
	FORCEINLINE void Sanitize()
	{
		WBench = FMath::Max(0.0f, WBench);
		WMonument = FMath::Max(0.0f, WMonument);
		WFastFood = FMath::Max(0.0f, WFastFood);
		PriceSensitivity = FMath::Clamp(PriceSensitivity, 0.0f, 1.0f);
	}
};
