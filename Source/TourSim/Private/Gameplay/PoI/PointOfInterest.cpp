// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/PoI/PointOfInterest.h"

#include "MassEntityTypes.h"


APointOfInterest::APointOfInterest()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APointOfInterest::BeginPlay()
{
	Super::BeginPlay();
	
}

void APointOfInterest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APointOfInterest::InitializeFromData(const FPoiConfiguration& Config)
{
}

void APointOfInterest::OnMassAgentArrived(FMassEntityHandle AgentHandle)
{
}

void APointOfInterest::OnMassAgentDeparted(FMassEntityHandle AgentHandle)
{
}

