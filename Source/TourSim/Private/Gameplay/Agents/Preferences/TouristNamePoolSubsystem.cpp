// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/TouristNamePoolSubsystem.h"

#include "Gameplay/Agents/Preferences/TouristNamePoolDataAsset.h"

void UTouristNamePoolSubsystem::RegisterPool(FName PoolId, TSoftObjectPtr<UTouristNamePoolDataAsset> PoolAsset)
{
	if (PoolId.IsNone())
	{
		return;
	}

	PoolAssets.FindOrAdd(PoolId) = PoolAsset;

	LoadedPools.Remove(PoolId);
}

bool UTouristNamePoolSubsystem::EnsurePoolLoaded(FName PoolId)
{
	if (PoolId.IsNone())
	{
		return false;
	}

	if (TObjectPtr<UTouristNamePoolDataAsset>* Existing = LoadedPools.Find(PoolId))
	{
		return (*Existing) != nullptr;
	}

	const TSoftObjectPtr<UTouristNamePoolDataAsset>* SoftPtr = PoolAssets.Find(PoolId);
	if (!SoftPtr || SoftPtr->IsNull())
	{
		return false;
	}

	UTouristNamePoolDataAsset* Loaded = SoftPtr->LoadSynchronous();
	if (!Loaded)
	{
		return false;
	}

	LoadedPools.Add(PoolId, Loaded);
	return true;
}

int32 UTouristNamePoolSubsystem::GetPoolSize(FName PoolId) const
{
	const TObjectPtr<UTouristNamePoolDataAsset>* Pool = LoadedPools.Find(PoolId);
	if (!Pool || !(*Pool))
	{
		return 0;
	}
	return (*Pool)->Names.Num();
}

bool UTouristNamePoolSubsystem::TryGetName(FName PoolId, int32 NameIndex, FString& OutName) const
{
	const TObjectPtr<UTouristNamePoolDataAsset>* Pool = LoadedPools.Find(PoolId);
	if (!Pool || !(*Pool))
	{
		return false;
	}

	const int32 Num = (*Pool)->Names.Num();
	if (Num <= 0)
	{
		return false;
	}

	const int32 SafeIndex = (NameIndex >= 0) ? (NameIndex % Num) : (FMath::Abs(NameIndex) % Num);
	OutName = (*Pool)->Names[SafeIndex];
	return true;
}
