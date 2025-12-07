// RLOPlayerController.cpp

#include "RLOPlayerController.h"

ARLOPlayerController::ARLOPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
}

void ARLOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set input mode to UI and Game
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}
