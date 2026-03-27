// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeatmapGridActor.generated.h"

UENUM(BlueprintType)
enum class EHeatmapMode : uint8
{
	Density	UMETA(DisplayName = "Density"),
	Noise	UMETA(DisplayName = "Noise")
};

UCLASS()
class TOURSIM_API AHeatmapGridActor : public AActor
{
	GENERATED_BODY()

public:
	AHeatmapGridActor();

protected:
	virtual void BeginPlay() override;
	
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	
	UFUNCTION(BlueprintCallable, Category = "Heatmap")
	void UpdateHeatmap();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heatmap")
	TObjectPtr<UInstancedStaticMeshComponent> GridMesh;
	
	UPROPERTY(EditAnywhere, Category="Heatmap")
	float DensityMaxReference = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap")
	FVector2D WorldMin = FVector2D(-9000.f, -9000.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap")
	FVector2D WorldMax = FVector2D(9000.f, 9000.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap", meta = (ClampMin = "50.0"))
	float CellSize = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap")
	float GridZ = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap", meta = (ClampMin = "0.05"))
	float UpdateInterval = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap")
	EHeatmapMode Mode = EHeatmapMode::Density;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap")
	TSubclassOf<AActor> TouristActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap")
	bool bAutoUpdate = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heatmap")
	bool bHideZeroCells = false;
	
private:
	int32 NumX = 0;
	int32 NumY = 0;
	
	TArray<float> DensityValues;
	TArray<float> NoiseValues;
	
	FTimerHandle UpdateTimerHandle;
	
private:
	void BuildGrid();
	void ComputeDensity();
	void ComputeNoise();
	void ApplyToInstances();
	void ResetValues();
	bool WorldToCell(const FVector& WorldLocation, int32& OutX, int32& OutY) const;
	int32 GetIndex(int32 X, int32 Y) const;
	bool IsValidCell(int32 X, int32 Y) const;
	float GetAverageNeighborDensity(int32 X, int32 Y) const;
};
