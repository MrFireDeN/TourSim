// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "Gameplay/PoI/PoiTypes.h"
#include "TouristIntentFragment.generated.h"

/**
 * Stores the current chosen POI type and target POI index for a tourist.
 */
USTRUCT(BlueprintType)
struct FTouristIntentFragment : public FMassFragment
{
	GENERATED_BODY()

	// Type of the current intent (Bench/Monument/FastFood)
	UPROPERTY(BlueprintReadOnly, Category="TourSim")
	EPoiType CurrentIntent = EPoiType::None;

	// Runtime index of the target POI (from UPoiSubsystem), or -1 if none
	UPROPERTY(BlueprintReadOnly, Category="TourSim")
	int32 TargetPoiIndex = -1;

	// Last recorded utility value for this intent (for hysteresis comparison)
	UPROPERTY()
	float LastUtility = 0.0f;

	// Reset to no intent
	void Clear()
	{
		CurrentIntent = EPoiType::None;
		TargetPoiIndex = -1;
		LastUtility = 0.0f;
	}
};
