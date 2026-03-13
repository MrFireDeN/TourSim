// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "PoiTypes.h"
#include "PoiRecord.generated.h"

/**
 * Compact runtime representation of a point of interest stored in the POI subsystem.
 * This record is intended for fast read access from simulation code.
 */
USTRUCT(BlueprintType)
struct FPoiRecord
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Poi")
	int32 Id = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Poi")
	EPoiType Type = EPoiType::Bench;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Poi")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Poi", meta=(ClampMin="0.0"))
	float InteractionRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Poi", meta=(ClampMin="0"))
	int32 PriceCents = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Poi", meta=(ClampMin="1"))
	int32 Capacity = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Poi", meta=(ClampMin="0"))
	int32 TouristCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Poi")
	bool bEnabled = true;
	
public:
	FORCEINLINE bool IsAccessible() const
	{
		return bEnabled && TouristCount < Capacity;
	}
	
	FORCEINLINE bool IsFree() const
	{
		return PriceCents == 0;
	}
};
