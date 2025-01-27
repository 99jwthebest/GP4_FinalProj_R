// Fill out your copyright notice in the Description page of Project Settings.


#include "Gp4_Race_Final/GameMode/GM_RacingGame.h"
#include "Gp4_Race_Final/GameMode/RCheckpoint.h"
#include "Gp4_Race_Final/Gp4_Race_FinalPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"



AGM_RacingGame::AGM_RacingGame()
{
	StartCountdownDuration = 3.0f; // 3.0f
	StartTimerDuration = 10.0f; // 10.0f
}

void AGM_RacingGame::BeginPlay()
{
	Super::BeginPlay();

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARCheckpoint::StaticClass(), CheckpointActorArray);

	//// Now FoundActors contains all actors of class ARCheckpoint
	//for (AActor* Actor : CheckpointActorArray)
	//{
	//	ARCheckpoint* Checkpoint = Cast<ARCheckpoint>(Actor);
	//	if (Checkpoint->GetIsFinishLine())
	//	{
	//		// Do something with each checkpoint
	//		
	//	}
	//}

	//StartCountdown();
}

void AGM_RacingGame::GameStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Game has started!!!"))
	GetWorld()->GetTimerManager().SetTimer(TimeRemainingTimer, this, &AGM_RacingGame::LoseOutOfTime, StartTimerDuration, false);
	//GetWorld()->GetTimerManager().ClearTimer(TimeRemainingTimer); // this might not work

}

void AGM_RacingGame::GameEnd(bool Win)
{
	if(Win)
		UE_LOG(LogTemp, Warning, TEXT("You Win!!!"))
	else
		UE_LOG(LogTemp, Warning, TEXT("You Lose!!!!"))

}

void AGM_RacingGame::StartCountdown()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Countdown..."))
	//Delay Node(5.0f) Seconds in duration
	GetWorld()->GetTimerManager().SetTimer(StartCountdownGameHandle, this, &AGM_RacingGame::GameStart, StartCountdownDuration, false);
	//GetWorld()->GetTimerManager().ClearTimer(BeginGameHandle); // this might not work
}

void AGM_RacingGame::LoseOutOfTime()
{
	GameEnd(false);
}


void AGM_RacingGame::WinCrossTheFinishLine()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeRemainingTimer); // this might not work
	GameEnd(true);
}

void AGM_RacingGame::AddTimeToTimer(int TimeToAdd)
{
	float TimingToAdd = UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(GetWorld(), TimeRemainingTimer) + TimeToAdd;
	GetWorld()->GetTimerManager().SetTimer(TimeRemainingTimer, this, &AGM_RacingGame::LoseOutOfTime, TimingToAdd, false);
	// set Time remaining to the return value
	/*UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AGp4_Race_FinalPlayerController* VehiclePlayerController = Cast<AGp4_Race_FinalPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	VehiclePlayerController->VehicleUI*/
}
