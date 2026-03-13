// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "PoiTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "PoiSubsystem.generated.h"

struct FPoiRecord;
struct FNearestPoiSet;
class APointOfInterest;

/**
 * World subsystem that owns the compact runtime registry of all POIs in the current level.
 * Provides fast read access for simulation code and centralized occupancy updates.
 */
UCLASS()
class TOURSIM_API UPoiSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/**
	 * Rebuilds the entire runtime POI registry from authored level actors.
	 * Existing runtime records and type indices are discarded.
	 */
	UFUNCTION(BlueprintCallable, Category="TourSim|Poi")
	void RebuildRegistry();

	/**
	 * Returns a read-only view of all runtime POI records.
	 *
	 * @return Read-only array of all POIs currently registered in the world.
	 */
	UFUNCTION(BlueprintPure, Category="TourSim|Poi")
	const TArray<FPoiRecord>& GetAllPois() const { return Pois; }

	/**
	 * Attempts to find the nearest accessible POI of each type relative to a world-space position.
	 *
	 * @param FromLocation Query origin in world space.
	 * @param OutSet Output set of nearest accessible POIs grouped by type.
	 * @return True if at least one accessible POI was found.
	 */
	UFUNCTION(BlueprintCallable, Category="TourSim|Poi")
	bool TryGetNearestAccessiblePoiPerType(const FVector& FromLocation, FNearestPoiSet& OutSet) const;

	/**
	 * Attempts to find the nearest accessible POI of the specified type.
	 *
	 * @param Type Requested POI type.
	 * @param FromLocation Query origin in world space.
	 * @param OutPoi Output pointer to the nearest accessible runtime POI record.
	 * @return True if a matching accessible POI was found.
	 */
	bool TryGetNearestAccessiblePoiOfType(EPoiType Type, const FVector& FromLocation, const FPoiRecord*& OutPoi) const;

	/**
	 * Returns a read-only pointer to a POI by runtime index.
	 *
	 * @param PoiIndex Runtime POI index.
	 * @return Pointer to the POI record or nullptr if index is invalid.
	 */
	const FPoiRecord* GetPoiByIndex(int32 PoiIndex) const;

	/**
	 * Returns whether the runtime POI index is valid.
	 *
	 * @param PoiIndex Runtime POI index.
	 * @return True if the index refers to an existing POI record.
	 */
	UFUNCTION(BlueprintPure, Category="TourSim|Poi")
	bool IsValidPoiIndex(int32 PoiIndex) const;

	/**
	 * Increments current tourist count for the specified POI if it is valid and not full.
	 *
	 * @param PoiIndex Runtime POI index.
	 * @return True if the count was incremented.
	 */
	UFUNCTION(BlueprintCallable, Category="TourSim|Poi")
	bool TryEnterPoi(int32 PoiIndex);

	/**
	 * Decrements current tourist count for the specified POI if it is valid and non-empty.
	 *
	 * @param PoiIndex Runtime POI index.
	 * @return True if the count was decremented.
	 */
	UFUNCTION(BlueprintCallable, Category="TourSim|Poi")
	bool TryLeavePoi(int32 PoiIndex);

	/**
	 * Returns whether the specified POI currently has free capacity.
	 *
	 * @param PoiIndex Runtime POI index.
	 * @return True if the POI exists and is accessible.
	 */
	UFUNCTION(BlueprintPure, Category="TourSim|Poi")
	bool IsPoiAccessible(int32 PoiIndex) const;

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	/**
	 * Registers one authored actor into the compact runtime registry.
	 *
	 * @param PoiActor Source authored actor.
	 * @param InId Runtime POI identifier.
	 */
	void RegisterPoiActor(const APointOfInterest& PoiActor, int32 InId);

	/**
	 * Returns the type-specific index array for iteration.
	 *
	 * @param Type Requested POI type.
	 * @return Read-only array of runtime POI indices for the requested type.
	 */
	const TArray<int32>& GetIndicesByType(EPoiType Type) const;

	/** Compact runtime registry of all authored POIs in the current world. */
	UPROPERTY()
	TArray<FPoiRecord> Pois;

	/** Runtime POI indices grouped by Bench type. */
	UPROPERTY()
	TArray<int32> BenchIndices;

	/** Runtime POI indices grouped by Monument type. */
	UPROPERTY()
	TArray<int32> MonumentIndices;

	/** Runtime POI indices grouped by FastFood type. */
	UPROPERTY()
	TArray<int32> FastFoodIndices;
};
