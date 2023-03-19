// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass); //create the HUD
    if (HUD != nullptr)
    {
        HUD->AddToViewport(); //add HUD to viewport
    }
}

void AShooterPlayerController::GameHasEnded(class AActor *EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport(); //remove the HUD

    if (bIsWinner)
    {
        UUserWidget *WinScreen = CreateWidget(this, WinScreenClass); //the win screen
        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget *LoseScreen = CreateWidget(this, LoseScreenClass); //represents the lose screen widget

        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }

    //After 5 seconds restart the level
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

