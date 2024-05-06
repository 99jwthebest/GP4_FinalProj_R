// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RCheckpoint.generated.h"

//class USpringArmComponent;

UCLASS()
class GP4_RACE_FINAL_API ARCheckpoint : public AActor
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//USpringArmComponent* CameraBoom;
	
public:	
	// Sets default values for this actor's properties
	ARCheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Game Mode")
		bool IsFinishLine;
	UPROPERTY(EditAnywhere, Category = "Game Mode")
		int TimeToGive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
		int NumberOfUses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
		int CurrentNumUsed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Game Mode")
		bool GetIsFinishLine();
	UFUNCTION(BlueprintPure, Category = "Game Mode")
		int GetTimeToGive();
};
