// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "TouristIntent.h"
#include "TouristStateFragment.generated.h"

/**
 * Fragment that stores the tourist's dynamic internal state and lightweight telemetry.
 * Kept separate from preferences to isolate "mutable runtime state" from "static profile".
 */
USTRUCT(BlueprintType)
struct FTouristStateFragment final : public FMassFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Needs", meta=(ClampMin="0.0", ClampMax="1.0"))
	float RestNeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Needs", meta=(ClampMin="0.0", ClampMax="1.0"))
	float CultureNeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Needs", meta=(ClampMin="0.0", ClampMax="1.0"))
	float HungerNeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Economy", meta=(ClampMin="0"))
	int32 MoneyCents = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Economy", meta=(ClampMin="0"))
	int32 SpentCents = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Decision")
	float NextDecisionTimeSeconds = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Decision")
	ETouristIntent CurrentIntent = ETouristIntent::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Stats")
	int32 BenchVisits = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Stats")
	int32 MonumentVisits = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Stats")
	int32 FastFoodPurchases = 0;

public:
	FORCEINLINE void Sanitize()
	{
		RestNeed = FMath::Clamp(RestNeed, 0.0f, 1.0f);
		CultureNeed = FMath::Clamp(CultureNeed, 0.0f, 1.0f);
		HungerNeed = FMath::Clamp(HungerNeed, 0.0f, 1.0f);
		MoneyCents = FMath::Max(0, MoneyCents);
		SpentCents = FMath::Max(0, SpentCents);
	}
};
