// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gp4_Race_FinalWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UGp4_Race_FinalWheelRear::UGp4_Race_FinalWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}