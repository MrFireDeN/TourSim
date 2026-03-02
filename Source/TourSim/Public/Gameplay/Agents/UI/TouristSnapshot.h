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

	UPROPERTY(BlueprintReadOnly)
	int32 ShortId = -1;

	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(BlueprintReadOnly)
	int32 MoneyCents = 0;

	UPROPERTY(BlueprintReadOnly)
	float RestNeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float CultureNeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float HungerNeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float WBench = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float WMonument = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float WFastFood = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float PriceSensitivity = 0.f;

	UPROPERTY(BlueprintReadOnly)
	uint8 SpawnSource = 0;
};
