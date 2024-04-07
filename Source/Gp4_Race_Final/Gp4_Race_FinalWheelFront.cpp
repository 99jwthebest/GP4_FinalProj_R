// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gp4_Race_FinalWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UGp4_Race_FinalWheelFront::UGp4_Race_FinalWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}