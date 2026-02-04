// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/PoI/PointOfInterest.h"
#include "Gameplay/PoI/PointOfInterest.h"

#include "MassEntityTypes.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/PoI/PoiDataComponent.h"


APointOfInterest::APointOfInterest()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	InfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("InfoWidgetComponent");
	InfoWidgetComponent->SetupAttachment(RootComponent);
	
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>("InteractionVolume");
	InteractionVolume->SetupAttachment(RootComponent);
	
	DataComponent = CreateDefaultSubobject<UPoiDataComponent>("DataComponent");
	
	// MassAgentSubsystem = CreateDefaultSubobject<UMassAgentSubsystem>("");
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

