// © 2026 Denis Sviridov. MIT License.


#include "GameFramework/TourSimGameMode.h"

#include "GameFramework/TourSimGameState.h"
#include "GameFramework/TourSimHUD.h"
#include "GameFramework/TourSimPlayerController.h"
#include "GameFramework/TourSimPlayerState.h"
#include "Gameplay/Pawn/TourSimPawn.h"

ATourSimGameMode::ATourSimGameMode()
{
	GameStateClass = ATourSimGameState::StaticClass();
	DefaultPawnClass = ATourSimPawn::StaticClass();
	PlayerControllerClass = ATourSimPlayerController::StaticClass();
	PlayerStateClass = ATourSimPlayerState::StaticClass();
	HUDClass = ATourSimHUD::StaticClass();
}

void ATourSimGameMode::BeginPlay()
{
	Super::BeginPlay();
}
