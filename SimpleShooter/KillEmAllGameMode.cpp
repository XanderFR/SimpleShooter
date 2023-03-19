// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController()); //who was controlling the character who died in the game; get controller of pawnkilled
    if (PlayerController != nullptr) //if someone was controlling the defeated character
    {
        //game over for the player
        //PlayerController->GameHasEnded(nullptr, false);
        //nullptr - what actor should we focus on in the end game; feature not needed
        //false - is this the winner; this is not the winning player controller
        EndGame(false); //where the player loses, player is not the winner
    }

    //For loop over shooter AI in the world
    //Is it not dead?
    //early return
    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!Controller->IsDead())
        {
            return;
        }
    }
    
    //if there are no living AI in the world, end the game
    EndGame(true); // the player has won
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
    for (AController* Controller : TActorRange<AController>(GetWorld())) //for all controllers in a list of controllers in the world
    {
        //true if player controller and bIsPlayerWinnrer (Player wins)
        //true if not player controller and not bIsPlayerWinner (AI wins)
        //false if player controller and not bIsPlayerWinner (Player loses)
        //false if not player controller and bIsPlayerWinner (AI loses)
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner; //is the player controller the winner
        //is this thing (Controller->IsPlayerController()) in the world the player controller
        //and is the player controller the winner

        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner); //focus on player pawn whether we win or lose
    }
}

