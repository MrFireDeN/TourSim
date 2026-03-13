// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/PoI/PoiSubsystem.h"

#include "EngineUtils.h"
#include "Gameplay/PoI/NearestPoiSet.h"
#include "Gameplay/PoI/PointOfInterest.h"
#include "Gameplay/PoI/PoiRecord.h"

void UPoiSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	RebuildRegistry();
}

void UPoiSubsystem::RebuildRegistry()
{
	Pois.Reset();
	BenchIndices.Reset();
	MonumentIndices.Reset();
	FastFoodIndices.Reset();
	
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	int32 NextId = 0;
	
	for (TActorIterator<APointOfInterest> It(World); It; ++It)
	{
		if (const APointOfInterest* PoiActor = *It)
		{
			RegisterPoiActor(*PoiActor, NextId);
			++NextId;
		}
	}
}

bool UPoiSubsystem::TryGetNearestAccessiblePoiPerType(const FVector& FromLocation, FNearestPoiSet& OutSet) const
{
	OutSet = FNearestPoiSet{};
	
	const bool bHasBench = TryGetNearestAccessiblePoiOfType(EPoiType::Bench, FromLocation, OutSet.Bench);
	const bool bHasMonument = TryGetNearestAccessiblePoiOfType(EPoiType::Monument, FromLocation, OutSet.Monument);
	const bool bHasFastFood = TryGetNearestAccessiblePoiOfType(EPoiType::FastFood, FromLocation, OutSet.FastFood);

	return bHasBench || bHasMonument || bHasFastFood;
}

bool UPoiSubsystem::TryGetNearestAccessiblePoiOfType(
	EPoiType Type, 
	const FVector& FromLocation,
	const FPoiRecord*& OutPoi) const
{
	OutPoi = nullptr;
	
	const TArray<int32>& IndicesByType = GetIndicesByType(Type);
	if (IndicesByType.IsEmpty())
	{
		return false;
	}
	
	float BestDistanceSq = TNumericLimits<float>::Max();
	
	for (const int32 PoiIndex : IndicesByType)
	{
		if (!Pois.IsValidIndex(PoiIndex))
		{
			continue;
		}
		
		const FPoiRecord& Poi = Pois[PoiIndex];
		if (!Poi.IsAccessible())
		{
			continue;
		}
		
		const float DistanceSq = FVector::DistSquared(FromLocation, Poi.Location);
		if (DistanceSq < BestDistanceSq)
		{
			BestDistanceSq = DistanceSq;
			OutPoi = &Poi;
		}
	}
	
	return OutPoi != nullptr;
}

const FPoiRecord* UPoiSubsystem::GetPoiByIndex(const int32 PoiIndex) const
{
	return Pois.IsValidIndex(PoiIndex) ? &Pois[PoiIndex] : nullptr;
}

bool UPoiSubsystem::IsValidPoiIndex(const int32 PoiIndex) const
{
	return Pois.IsValidIndex(PoiIndex);
}

bool UPoiSubsystem::TryEnterPoi(const int32 PoiIndex)
{
	if (!Pois.IsValidIndex(PoiIndex))
	{
		return false;
	}
	
	FPoiRecord& Poi = Pois[PoiIndex];
	if (!Poi.IsAccessible())
	{
		return false;
	}
	
	++Poi.TouristCount;
	return true;
}

bool UPoiSubsystem::TryLeavePoi(const int32 PoiIndex)
{
	if (!Pois.IsValidIndex(PoiIndex))
	{
		return false;
	}
	
	FPoiRecord& Poi = Pois[PoiIndex];
	if (Poi.TouristCount <= 0)
	{
		return false;
	}
	
	--Poi.TouristCount;
	return true;
}

bool UPoiSubsystem::IsPoiAccessible(const int32 PoiIndex) const
{
	return Pois.IsValidIndex(PoiIndex) && Pois[PoiIndex].IsAccessible();
}

void UPoiSubsystem::RegisterPoiActor(const APointOfInterest& PoiActor, const int32 InId)
{
	FPoiRecord Record = PoiActor.BuildPoiRecord(InId);
	
	const int32 NewIndex = Pois.Add(MoveTemp(Record));
	const EPoiType Type = Pois[NewIndex].Type;
	
	switch (Type)
	{
	case EPoiType::Bench:
		BenchIndices.Add(NewIndex);
		break;
		
	case EPoiType::Monument:
		MonumentIndices.Add(NewIndex);
		break;
		
	case EPoiType::FastFood:
		FastFoodIndices.Add(NewIndex);
		break;
		
	default:
		break;
	}
}

const TArray<int32>& UPoiSubsystem::GetIndicesByType(EPoiType Type) const
{
	switch (Type)
	{
	case EPoiType::Bench:
		return BenchIndices;

	case EPoiType::Monument:
		return MonumentIndices;

	case EPoiType::FastFood:
		return FastFoodIndices;

	default:
		return BenchIndices;
	}
}
