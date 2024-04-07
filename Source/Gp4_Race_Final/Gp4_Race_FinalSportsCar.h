// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gp4_Race_FinalPawn.h"
#include "Gp4_Race_FinalSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class GP4_RACE_FINAL_API AGp4_Race_FinalSportsCar : public AGp4_Race_FinalPawn
{
	GENERATED_BODY()
	
public:

	AGp4_Race_FinalSportsCar();
};
