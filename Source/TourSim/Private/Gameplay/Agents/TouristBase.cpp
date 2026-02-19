// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/TouristBase.h"

#include "MassAgentComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"


ATouristBase::ATouristBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	
	CapsuleComponent->InitCapsuleSize(30.0f, 89.0f);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetGenerateOverlapEvents(false);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -89.0f));
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);
	
	MassAgentComponent = CreateDefaultSubobject<UMassAgentComponent>("MassAgentComponent");
	
#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->SetHiddenInGame(true);
#endif
}

void ATouristBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATouristBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
