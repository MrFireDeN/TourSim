// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Spawn/AgentSpawnSourceBase.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


AAgentSpawnSourceBase::AAgentSpawnSourceBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;

	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetBoxExtent(FVector(200.f, 200.f, 100.f));
}

void AAgentSpawnSourceBase::BeginPlay()
{
	Super::BeginPlay();
	
	// GetWorld()->GetSubsystem<UTourSimSpawnSubsystem>()->RegisterSource(this);
}

void AAgentSpawnSourceBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// if (auto* Subsys = GetWorld()->GetSubsystem<UTourSimSpawnSubsystem>())
	// {
	//     Subsys->UnregisterSource(this);
	// }
	
	Super::EndPlay(EndPlayReason);
}

void AAgentSpawnSourceBase::AccumulateDemand(const float DeltaSeconds)
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

int32 AAgentSpawnSourceBase::ComputeSpawnQuota(const int32 GlobalBudget)
{
	if (GlobalBudget <= 0)
	{
		return 0;
	}
	
	const int32 LocalAvailable = FMath::FloorToInt(PendingQueue);
	const int32 AllowedByTick  = FMath::Min(MaxSpawnPerTick, LocalAvailable);

	return FMath::Min(AllowedByTick, GlobalBudget);
}

bool AAgentSpawnSourceBase::TryConsumeOne()
{
	if (PendingQueue >= 1.0f)
	{
		PendingQueue -= 1.0f;
		return true;
	}
	return false;
}

bool AAgentSpawnSourceBase::GetCandidateTransform(FTransform& OutTransform) const
{
	if (!SpawnArea)
	{
		return false;
	}
	
	const FVector Origin = SpawnArea->GetComponentLocation();
	const FVector Extent = SpawnArea->GetScaledBoxExtent();

	const FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	OutTransform = FTransform(GetActorRotation(), RandomPoint, FVector::OneVector);
	return true;
}
