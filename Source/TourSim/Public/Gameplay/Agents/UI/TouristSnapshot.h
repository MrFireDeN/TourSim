// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "TouristSnapshot.generated.h"

/**
 * Read-only snapshot of tourist state for UI/debug purposes.
 * Pure data transfer object (DTO).
 */
USTRUCT(BlueprintType)
struct FTouristSnapshot
{
	GENERATED_BODY()
	
	FTouristSnapshot() = default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShortId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MoneyCents = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RestNeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CultureNeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HungerNeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WBench = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WMonument = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WFastFood = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PriceSensitivity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SpawnSource = 0;
};
