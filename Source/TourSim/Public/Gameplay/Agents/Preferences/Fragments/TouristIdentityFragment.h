// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "TouristIdentityFragment.generated.h"

/**
 * Fragment for UI-facing identity fields.
 *
 * Notes:
 * - Store NameIndex (not FString) to keep fragments lightweight.
 * - Store ShortId for compact on-screen display.
 * - PoolId is stored in a const shared fragment (archetype config), not per-entity.
 */
USTRUCT(BlueprintType)
struct FTouristIdentityFragment final : public FMassFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Identity")
	int32 NameIndex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Identity")
	int32 ShortId = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Identity")
	int32 IdHash = 0;
};
