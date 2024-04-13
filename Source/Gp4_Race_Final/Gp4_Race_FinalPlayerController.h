// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Gp4_Race_FinalPlayerController.generated.h"

class UInputMappingContext;
class AGp4_Race_FinalPawn;
class UGp4_Race_FinalUI;

/**
 *  Vehicle Player Controller class
 *  Handles input mapping and user interface
 */
UCLASS(abstract)
class GP4_RACE_FINAL_API AGp4_Race_FinalPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	/** Pointer to the controlled vehicle pawn */
	TObjectPtr<AGp4_Race_FinalPawn> VehiclePawn;

	/** Type of the UI to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UGp4_Race_FinalUI> VehicleUIClass;


	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

public:
	/** Pointer to the UI widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<UGp4_Race_FinalUI> VehicleUI;

	virtual void Tick(float Delta) override;

	// End Actor interface

	// Begin PlayerController interface
protected:

	virtual void OnPossess(APawn* InPawn) override;

	// End PlayerController interface
};
