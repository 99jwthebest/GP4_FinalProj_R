// Fill out your copyright notice in the Description page of Project Settings.


#include "Gp4_Race_Final/GameMode/RCheckpoint.h"
//#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARCheckpoint::ARCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	IsFinishLine = false;
	TimeToGive = 0;
}

// Called when the game starts or when spawned
void ARCheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ARCheckpoint::GetIsFinishLine()
{
	if(IsFinishLine)
		return true;

	return false;
}

int ARCheckpoint::GetTimeToGive()
{
	return TimeToGive;
}

