// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "PoiTypes.h"
#include "GameFramework/Actor.h"
#include "PointOfInterest.generated.h"


struct FPoiRecord;
class UBillboardComponent;
class USphereComponent;
class UStaticMeshComponent;

/**
 * Authoring actor for a point of interest placed in the level.
 * Stores editable POI parameters and can export them into a compact runtime record.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TOURSIM_API APointOfInterest : public AActor
{
	GENERATED_BODY()

public:
	APointOfInterest();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> InteractionVolume;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UBillboardComponent> Billboard;
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Poi")
	EPoiType Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Poi", meta=(ClampMin="0"))
	int32 PriceCents = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Poi", meta=(ClampMin="1"))
	int32 Capacity = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TourSim|Poi", meta=(ClampMin="0"))
	int32 TouristCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Poi")
	bool bEnabled = true;
	
public:
	/**
	 * Builds a runtime POI record from the actor's current authored state.
	 *
	 * @param InId Stable runtime identifier assigned by the subsystem.
	 * @return Compact runtime POI record.
	 */
	UFUNCTION(BlueprintCallable, Category="TourSim|Poi")
	FPoiRecord BuildPoiRecord(int32 InId) const;
	
	UFUNCTION(BlueprintPure, Category="TourSim|Poi")
	bool IsAccessible() const;

	UFUNCTION(BlueprintPure, Category="TourSim|Poi")
	int32 GetTouristCount() const
	{
		return TouristCount;
	}

	UFUNCTION(BlueprintCallable, Category="TourSim|Poi")
	void SetTouristCount(int32 InTouristCount);
};
