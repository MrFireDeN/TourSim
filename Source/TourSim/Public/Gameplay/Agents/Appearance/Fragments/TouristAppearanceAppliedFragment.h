// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "TouristAppearanceAppliedFragment.generated.h"


USTRUCT()
struct FTouristAppearanceAppliedFragment : public FMassFragment
{
	GENERATED_BODY()

	FObjectKey LastActorKey;
	uint32 LastColorHash = 0;
};
