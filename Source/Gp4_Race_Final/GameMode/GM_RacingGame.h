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
	void StartCountdown();
	void LoseOutOfTime();
	void WinCrossTheFinishLine();



	//UPROPERTY(VisibleAnywhere, Category = "Game Mode")
	bool PlayerWon;
	//UPROPERTY(EditAnywhere, Category = Movement)
	FTimerHandle StartCountdownGameHandle;
	float StartCountdownDuration;
	FTimerHandle TimeRemainingTimer;
	float StartTimerDuration;


};
