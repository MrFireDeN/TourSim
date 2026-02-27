// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "TouristIntent.generated.h"

/**
 * High-level intent of a tourist agent for the current decision horizon.
 * Used for debugging, telemetry, and lightweight behavior branching.
 */
UENUM(BlueprintType)
enum class ETouristIntent : uint8
{
	None    UMETA(DisplayName = "None"),
	Rest    UMETA(DisplayName = "Rest"),
	Culture UMETA(DisplayName = "Culture"),
	Eat     UMETA(DisplayName = "Eat")
};
