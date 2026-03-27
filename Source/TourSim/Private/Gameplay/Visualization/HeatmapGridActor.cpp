// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Visualization/HeatmapGridActor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


AHeatmapGridActor::AHeatmapGridActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GridMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GridMesh"));
	SetRootComponent(GridMesh);

	GridMesh->SetMobility(EComponentMobility::Movable);
	GridMesh->NumCustomDataFloats = 1;
	GridMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GridMesh->CastShadow = false;
}

void AHeatmapGridActor::BeginPlay()
{
	Super::BeginPlay();
	
	BuildGrid();

	if (bAutoUpdate && UpdateInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(
			UpdateTimerHandle,
			this,
			&AHeatmapGridActor::UpdateHeatmap,
			UpdateInterval,
			true);
	}

	UpdateHeatmap();
}

#if WITH_EDITOR
void AHeatmapGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BuildGrid();
	ApplyToInstances();
}
#endif

void AHeatmapGridActor::BuildGrid()
{
	if (!GridMesh)
	{
		return;
	}

	GridMesh->ClearInstances();

	const float Width = FMath::Max(0.f, WorldMax.X - WorldMin.X);
	const float Height = FMath::Max(0.f, WorldMax.Y - WorldMin.Y);

	NumX = FMath::Max(1, FMath::CeilToInt(Width / CellSize));
	NumY = FMath::Max(1, FMath::CeilToInt(Height / CellSize));

	const int32 TotalCells = NumX * NumY;

	DensityValues.SetNumZeroed(TotalCells);
	NoiseValues.SetNumZeroed(TotalCells);

	const FVector ActorLocation = GetActorLocation();

	for (int32 Y = 0; Y < NumY; ++Y)
	{
		for (int32 X = 0; X < NumX; ++X)
		{
			const float CellCenterX = WorldMin.X + (X + 0.5f) * CellSize;
			const float CellCenterY = WorldMin.Y + (Y + 0.5f) * CellSize;

			const FVector WorldPosition(CellCenterX, CellCenterY, GridZ);
			const FVector LocalPosition = WorldPosition - ActorLocation;

			const FVector Scale(CellSize / 100.f, CellSize / 100.f, 1.f);
			const FTransform InstanceTransform(FRotator::ZeroRotator, LocalPosition, Scale);

			const int32 InstanceIndex = GridMesh->AddInstance(InstanceTransform);
			GridMesh->SetCustomDataValue(InstanceIndex, 0, 0.f, false);
		}
	}

	GridMesh->MarkRenderStateDirty();
}

void AHeatmapGridActor::UpdateHeatmap()
{
	if (!GridMesh || NumX <= 0 || NumY <= 0)
	{
		return;
	}

	ResetValues();
	ComputeDensity();

	if (Mode == EHeatmapMode::Noise)
	{
		ComputeNoise();
	}

	ApplyToInstances();
}

void AHeatmapGridActor::ComputeDensity()
{
	if (!GetWorld())
	{
		return;
	}

	TArray<AActor*> TouristActors;

	if (TouristActorClass)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), TouristActorClass, TouristActors);
	}
	else
	{
		return;
	}

	for (AActor* TouristActor : TouristActors)
	{
		if (!IsValid(TouristActor))
		{
			continue;
		}

		int32 CellX = 0;
		int32 CellY = 0;
		if (!WorldToCell(TouristActor->GetActorLocation(), CellX, CellY))
		{
			continue;
		}

		const int32 Index = GetIndex(CellX, CellY);
		if (DensityValues.IsValidIndex(Index))
		{
			DensityValues[Index] += 1.f;
		}
	}
}

void AHeatmapGridActor::ComputeNoise()
{
	if (NoiseValues.Num() != DensityValues.Num())
	{
		return;
	}

	for (int32 Y = 0; Y < NumY; ++Y)
	{
		for (int32 X = 0; X < NumX; ++X)
		{
			const int32 Index = GetIndex(X, Y);
			const float CurrentDensity = DensityValues[Index];
			const float AverageNeighborDensity = GetAverageNeighborDensity(X, Y);

			NoiseValues[Index] = FMath::Abs(CurrentDensity - AverageNeighborDensity);
		}
	}
}

void AHeatmapGridActor::ApplyToInstances()
{
	if (!GridMesh)
	{
		return;
	}

	const TArray<float>& SourceValues = (Mode == EHeatmapMode::Density) ? DensityValues : NoiseValues;
	if (SourceValues.Num() <= 0)
	{
		return;
	}

	const float InvMax = (DensityMaxReference > KINDA_SMALL_NUMBER)
	? (1.f / DensityMaxReference)
	: 0.f;

	const int32 InstanceCount = GridMesh->GetInstanceCount();
	const int32 ValueCount = SourceValues.Num();
	const int32 SafeCount = FMath::Min(InstanceCount, ValueCount);

	for (int32 Index = 0; Index < SafeCount; ++Index)
	{
		float NormalizedValue = SourceValues[Index] * InvMax;
		NormalizedValue = FMath::Clamp(NormalizedValue, 0.f, 1.f);

		if (bHideZeroCells && SourceValues[Index] <= KINDA_SMALL_NUMBER)
		{
			NormalizedValue = 0.f;
		}

		GridMesh->SetCustomDataValue(Index, 0, NormalizedValue, false);
	}

	GridMesh->MarkRenderStateDirty();
}

void AHeatmapGridActor::ResetValues()
{
	const int32 TotalCells = NumX * NumY;

	if (DensityValues.Num() != TotalCells)
	{
		DensityValues.SetNumZeroed(TotalCells);
	}
	else
	{
		for (float& Value : DensityValues)
		{
			Value = 0.f;
		}
	}

	if (NoiseValues.Num() != TotalCells)
	{
		NoiseValues.SetNumZeroed(TotalCells);
	}
	else
	{
		for (float& Value : NoiseValues)
		{
			Value = 0.f;
		}
	}
}

bool AHeatmapGridActor::WorldToCell(const FVector& WorldLocation, int32& OutX, int32& OutY) const
{
	if (WorldLocation.X < WorldMin.X || WorldLocation.X >= WorldMax.X ||
		WorldLocation.Y < WorldMin.Y || WorldLocation.Y >= WorldMax.Y)
	{
		return false;
	}

	OutX = FMath::FloorToInt((WorldLocation.X - WorldMin.X) / CellSize);
	OutY = FMath::FloorToInt((WorldLocation.Y - WorldMin.Y) / CellSize);

	return IsValidCell(OutX, OutY);
}

int32 AHeatmapGridActor::GetIndex(int32 X, int32 Y) const
{
	return Y * NumX + X;
}

bool AHeatmapGridActor::IsValidCell(int32 X, int32 Y) const
{
	return X >= 0 && X < NumX && Y >= 0 && Y < NumY;
}

float AHeatmapGridActor::GetAverageNeighborDensity(int32 X, int32 Y) const
{
	float Sum = 0.f;
	int32 Count = 0;

	for (int32 OffsetY = -1; OffsetY <= 1; ++OffsetY)
	{
		for (int32 OffsetX = -1; OffsetX <= 1; ++OffsetX)
		{
			if (OffsetX == 0 && OffsetY == 0)
			{
				continue;
			}

			const int32 NeighborX = X + OffsetX;
			const int32 NeighborY = Y + OffsetY;

			if (!IsValidCell(NeighborX, NeighborY))
			{
				continue;
			}

			Sum += DensityValues[GetIndex(NeighborX, NeighborY)];
			++Count;
		}
	}

	if (Count <= 0)
	{
		return 0.f;
	}

	return Sum / static_cast<float>(Count);
}
