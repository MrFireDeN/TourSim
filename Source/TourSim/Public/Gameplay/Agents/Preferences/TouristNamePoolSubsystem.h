// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TouristNamePoolSubsystem.generated.h"

class UTouristNamePoolDataAsset;
/**
 * World subsystem that owns and serves name pools for UI and spawn-time identity assignment.
 *
 * Design:
 * - Pools are registered by (PoolId, AssetPtr).
 * - Assets are loaded synchronously on first use
 * - Fragments store only NameIndex + ShortId; actual strings are resolved here.
 */
UCLASS()
class TOURSIM_API UTouristNamePoolSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/** Registers a pool id with a data asset reference (safe to call repeatedly). */
	UFUNCTION(BlueprintCallable, Category="TourSim|Names")
	void RegisterPool(FName PoolId, TSoftObjectPtr<UTouristNamePoolDataAsset> PoolAsset);

	/** Ensures that a pool is loaded; returns true if loaded and non-null. */
	UFUNCTION(BlueprintCallable, Category="TourSim|Names")
	bool EnsurePoolLoaded(FName PoolId);

	/** Returns the number of names in a loaded pool, or 0 if missing/unloaded. */
	UFUNCTION(BlueprintCallable, Category="TourSim|Names")
	int32 GetPoolSize(FName PoolId) const;

	/**
	 * Resolves a name by index. If index is out of range, it is wrapped by modulo.
	 * Returns false if pool is missing/unloaded/empty.
	 */
	UFUNCTION(BlueprintCallable, Category="TourSim|Names")
	bool TryGetName(FName PoolId, int32 NameIndex, FString& OutName) const;
	
private:
	/** Soft references for pools (config-like). */
	UPROPERTY()
	TMap<FName, TSoftObjectPtr<UTouristNamePoolDataAsset>> PoolAssets;

	/** Loaded pools (runtime cache). */
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UTouristNamePoolDataAsset>> LoadedPools;
};
