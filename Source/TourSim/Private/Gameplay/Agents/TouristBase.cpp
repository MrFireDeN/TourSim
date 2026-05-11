// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/TouristBase.h"

#include "Components/ArrowComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/PawnMovementComponent.h"


ATouristBase::ATouristBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	Mesh->SetupAttachment(RootComponent);
}

void ATouristBase::BeginPlay()
{
	Super::BeginPlay();
	
	LastLocation = GetActorLocation();
	PrimaryActorTick.bCanEverTick = true;
}

void ATouristBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bFirstTick)
	{
		bFirstTick = false;
		return;
	}

	const FVector CurrentLocation = GetActorLocation();
	if (DeltaSeconds > SMALL_NUMBER)
	{
		CurrentVelocity = (CurrentLocation - LastLocation) / DeltaSeconds;
		
		if (CurrentVelocity.Size() > 0.1f)
		{
			const FRotator TargetRotation = CurrentVelocity.Rotation();
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, RotationSpeed));
		}
	}
	LastLocation = CurrentLocation;
}
