// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "PoiTypes.h"
#include "NearestPoiSet.generated.h"


struct FPoiRecord;

/**
 * Lightweight view of nearest accessible POIs grouped by domain type.
 * Stores non-owning pointers to subsystem records and is intended for transient query results only.
 */
USTRUCT(BlueprintType)
struct FNearestPoiSet
{
	GENERATED_BODY()

public:
	const FPoiRecord* Bench = nullptr;
	const FPoiRecord* Monument = nullptr;
	const FPoiRecord* FastFood = nullptr;

public:
	FORCEINLINE const FPoiRecord* Get(const EPoiType Type) const
	{
		switch (Type)
		{
		case EPoiType::Bench:
			return Bench;

		case EPoiType::Monument:
			return Monument;

		case EPoiType::FastFood:
			return FastFood;

		default:
			return nullptr;
		}
	}

	FORCEINLINE void Set(const EPoiType Type, const FPoiRecord* const Poi)
	{
		switch (Type)
		{
		case EPoiType::Bench:
			Bench = Poi;
			break;

		case EPoiType::Monument:
			Monument = Poi;
			break;

		case EPoiType::FastFood:
			FastFood = Poi;
			break;

		default:
			break;
		}
	}

	FORCEINLINE bool HasAny() const
	{
		return Bench != nullptr || Monument != nullptr || FastFood != nullptr;
	}
};