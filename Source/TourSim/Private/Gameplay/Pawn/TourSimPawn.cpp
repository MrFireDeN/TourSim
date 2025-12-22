// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Pawn/TourSimPawn.h"


ATourSimPawn::ATourSimPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATourSimPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATourSimPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATourSimPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

