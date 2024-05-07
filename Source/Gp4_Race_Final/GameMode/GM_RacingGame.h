// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_RacingGame.generated.h"

/**
 * 
 */
UCLASS()
class GP4_RACE_FINAL_API AGM_RacingGame : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGM_RacingGame();


protected:

	// Called when the game starts
	virtual void BeginPlay() override;



	//UFUNCTION(EditDefaultsOnly, Category = "Game Mode")
	void GameStart();
	void GameEnd(bool Win);
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
		void StartCountdown();
	void LoseOutOfTime();
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
		void WinCrossTheFinishLine();
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
		void AddTimeToTimer(int TimeToAdd);


	//UPROPERTY(VisibleAnywhere, Category = "Game Mode")
	bool PlayerWon;
	//UPROPERTY(EditAnywhere, Category = Movement)
	FTimerHandle StartCountdownGameHandle;
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
		float StartCountdownDuration;
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
		FTimerHandle TimeRemainingTimer;
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
		float StartTimerDuration;
	UPROPERTY(EditAnywhere)
		TArray<AActor*> CheckpointActorArray;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Input")
		bool MovementEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Input")
		bool GameIsOver = false;

};
