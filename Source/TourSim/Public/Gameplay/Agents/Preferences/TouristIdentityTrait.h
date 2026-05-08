// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"

#include "Fragments/TouristPreferencesFragment.h"
#include "Fragments/TouristStateFragment.h"
#include "Fragments/TouristIdentityFragment.h"
#include "Fragments/TouristSpawnSourceFragment.h"
#include "Fragments/TouristConfigSharedFragment.h"
#include "TouristIdentityTrait.generated.h"


class UTouristNamePoolDataAsset;

/**
 * Single trait that installs all tourist fragments and attaches archetype configuration as const shared data.
 *
 * Note:
 * - Per-entity randomization and spawn-source branching is performed by an observer processor
 *   triggered by FTouristNeedsInitTag (added by the spawner after it sets SpawnSource).
 */
UCLASS(DisplayName="Tourist Trait")
class TOURSIM_API UTouristIdentityTrait final : public UMassEntityTraitBase
{
	GENERATED_BODY()

protected:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
