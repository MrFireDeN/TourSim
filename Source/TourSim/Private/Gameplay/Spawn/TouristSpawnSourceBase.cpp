// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Spawn/TouristSpawnSourceBase.h"

#include "Components/BoxComponent.h"
#include "Gameplay/Spawn/TourSimSpawnLog.h"
#include "Gameplay/Spawn/TourSimSpawnSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTourSimSpawn);

ATouristSpawnSourceBase::ATouristSpawnSourceBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;

	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetBoxExtent(FVector(200.f, 200.f, 100.f));
}

void ATouristSpawnSourceBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (UTourSimSpawnSubsystem* Subsys = GetWorld()->GetSubsystem<UTourSimSpawnSubsystem>())
	{
		Subsys->RegisterSource(this);

		UE_LOG(LogTourSimSpawn, Warning,
			TEXT("[SpawnSource] Registered: %s  Type=%d"),
			*GetName(),
			static_cast<int32>(SourceType));
	}
}

void ATouristSpawnSourceBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UTourSimSpawnSubsystem* Subsys = GetWorld()->GetSubsystem<UTourSimSpawnSubsystem>())
	{
		Subsys->UnregisterSource(this);

		UE_LOG(LogTourSimSpawn, Warning,
			TEXT("[SpawnSource] Unregistered: %s"),
			*GetName());
	}
	
	Super::EndPlay(EndPlayReason);
}

void ATouristSpawnSourceBase::AccumulateDemand(const float DeltaSeconds)
{
	TokenBucket.Refill(DeltaSeconds);
	
	while (TokenBucket.Consume(1.0))
	{
		if (PendingQueue < MaxQueueSize)
		{
			PendingQueue += 1.0f;
		}
		else
		{
			break;
		}
	}
}

int32 ATouristSpawnSourceBase::ComputeSpawnQuota(const int32 GlobalBudget)
{
	if (GlobalBudget <= 0)
	{
		return 0;
	}
	
	const int32 LocalAvailable = FMath::FloorToInt(PendingQueue);
	const int32 AllowedByTick  = FMath::Min(MaxSpawnPerTick, LocalAvailable);

	return FMath::Min(AllowedByTick, GlobalBudget);
}

bool ATouristSpawnSourceBase::TryConsumeOne()
{
	if (PendingQueue >= 1.0f)
	{
		PendingQueue -= 1.0f;
		return true;
	}
	return false;
}

bool ATouristSpawnSourceBase::GetCandidateTransform(FTransform& OutTransform) const
{
	if (!SpawnArea)
	{
		return false;
	}
	
	const FVector Origin = SpawnArea->GetComponentLocation();
	const FVector Extent = SpawnArea->GetScaledBoxExtent();

	const FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	const FVector CandidateLocation = FVector(RandomPoint.X, RandomPoint.Y, 0);
	
	OutTransform = FTransform(GetActorRotation(), CandidateLocation, FVector::OneVector);
	return true;
}
