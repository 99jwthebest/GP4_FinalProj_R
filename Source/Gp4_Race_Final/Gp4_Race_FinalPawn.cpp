// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gp4_Race_FinalPawn.h"
#include "Gp4_Race_FinalWheelFront.h"
#include "Gp4_Race_FinalWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ChaosWheeledVehicleMovementComponent.h"

#define LOCTEXT_NAMESPACE "VehiclePawn"

DEFINE_LOG_CATEGORY(LogTemplateVehicle);

AGp4_Race_FinalPawn::AGp4_Race_FinalPawn()
{
	// construct the front camera boom
	FrontSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Front Spring Arm"));
	FrontSpringArm->SetupAttachment(GetMesh());
	FrontSpringArm->TargetArmLength = 0.0f;
	FrontSpringArm->bDoCollisionTest = false;
	FrontSpringArm->bEnableCameraRotationLag = true;
	FrontSpringArm->CameraRotationLagSpeed = 15.0f;
	FrontSpringArm->SetRelativeLocation(FVector(30.0f, 0.0f, 120.0f));

	FrontCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Front Camera"));
	FrontCamera->SetupAttachment(FrontSpringArm);
	FrontCamera->bAutoActivate = false;

	// construct the back camera boom
	BackSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Back Spring Arm"));
	BackSpringArm->SetupAttachment(GetMesh());
	BackSpringArm->TargetArmLength = 650.0f;
	BackSpringArm->SocketOffset.Z = 150.0f;
	BackSpringArm->bDoCollisionTest = false;
	BackSpringArm->bInheritPitch = false;
	BackSpringArm->bInheritRoll = false;
	BackSpringArm->bEnableCameraRotationLag = true;
	BackSpringArm->CameraRotationLagSpeed = 2.0f;
	BackSpringArm->CameraLagMaxDistance = 50.0f;

	BackCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Back Camera"));
	BackCamera->SetupAttachment(BackSpringArm);

	VehicleFastCameraShake = CreateDefaultSubobject<UCameraShakeSourceComponent>(TEXT("CameraShakeSourceCPP"));
	VehicleFastCameraShake->SetupAttachment(GetMesh());

	// Configure the car mesh
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	// get the Chaos Wheeled movement component
	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

	bCameraShakeStarted = false;
	NitrousLevel = 100.0f;

}

void AGp4_Race_FinalPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// steering 
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AGp4_Race_FinalPawn::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AGp4_Race_FinalPawn::Steering);

		// throttle 
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AGp4_Race_FinalPawn::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AGp4_Race_FinalPawn::Throttle);

		// break 
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AGp4_Race_FinalPawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &AGp4_Race_FinalPawn::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AGp4_Race_FinalPawn::StopBrake);

		// handbrake 
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AGp4_Race_FinalPawn::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AGp4_Race_FinalPawn::StopHandbrake);

		// look around 
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AGp4_Race_FinalPawn::LookAround);

		// toggle camera 
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &AGp4_Race_FinalPawn::ToggleCamera);

		// reset the vehicle 
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &AGp4_Race_FinalPawn::ResetVehicle);
	}
	else
	{
		UE_LOG(LogTemplateVehicle, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGp4_Race_FinalPawn::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(NitrousTimer, this, &AGp4_Race_FinalPawn::UpdateNitrousLevel, 0.2f, true); // updating nitrous with timer instead of update
}

void AGp4_Race_FinalPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// add some angular damping if the vehicle is in midair
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);

	/*float startMaxTorque = 750.0f;
	float NitrousMaxTorque = 1200.0f;
	ChaosVehicleMovement->SetMaxEngineTorque(NitrousMaxTorque);*/

	//ChaosVehicleMovement->SetMaxEngineTorque();

	// format the speed to KPH or MPH
	float FormattedSpeed = ChaosVehicleMovement->GetForwardSpeed() * (bIsMPHP ? 0.022f : 0.036f);
	//UE_LOG(LogTemp, Warning, TEXT("Default Formatted Speed: %f !!"), FormattedSpeed)

	float InterpFloatSpringArm = FMath::FInterpTo(BackSpringArm->CameraLagMaxDistance, (FormattedSpeed > 80.0f ? 10.0f : 1.0f), Delta, 0.5f);
	BackSpringArm->CameraLagMaxDistance = InterpFloatSpringArm;

	float InterpFloatBackCam = FMath::FInterpTo(BackCamera->FieldOfView, (FormattedSpeed > 80.0f ? 115.0f : 90.0f), Delta, 0.5f);
	BackCamera->SetFieldOfView(InterpFloatBackCam);

	if (FormattedSpeed > 80.0f)
	{
		if (!bCameraShakeStarted) 
		{
			VehicleFastCameraShake->Start();
			bCameraShakeStarted = true;
		}
	}
	else
	{
		bCameraShakeStarted = false;
		VehicleFastCameraShake->StopAllCameraShakes(false);
	}





	// realign the camera yaw to face front
	float CameraYaw = BackSpringArm->GetRelativeRotation().Yaw;
	CameraYaw = FMath::FInterpTo(CameraYaw, 0.0f, Delta, 1.0f);

	BackSpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 0.0f));
}

void AGp4_Race_FinalPawn::SetNitrousSpeed(float value)
{
	ChaosVehicleMovement->SetMaxEngineTorque(value);
	/*for(int i = 0; i < 3; i++)
		ChaosVehicleMovement->SetDriveTorque(value, i);*/
	//ChaosVehicleMovement->AddRadialImpulse(GetActorLocation(), 1000.0f, 1000000.0f, ERadialImpulseFalloff::RIF_Constant, false);
	UE_LOG(LogTemp, Warning, TEXT("MAX TORQUE awwwwwww: %f !!"), ChaosVehicleMovement->EngineSetup.MaxTorque)

}

bool AGp4_Race_FinalPawn::bNitrousLevel()
{
	if (NitrousLevel > 0.0f)
	{
		return true;
	}

	return false;
}

void AGp4_Race_FinalPawn::UpdateNitrousLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("NITROUS LEVEL: %f !!"), NitrousLevel)

	if (NitrousEnabled)
	{

		FVector PhysicsVelocity = GetMesh()->GetPhysicsLinearVelocity() * 10.0f;

		GetMesh()->SetPhysicsLinearVelocity(PhysicsVelocity, false);
		//GetMesh()->AddImpulse
		
		NitrousLevel -= 3.0f;
		float clampNitrous = FMath::Clamp(NitrousLevel, 0.0f, 100.0f);
		NitrousLevel = clampNitrous;
		if (NitrousLevel < 0.0f)
		{
			NitrousEnabled = false;
		}
	}
	else
	{
		NitrousLevel += 0.1f;
		float clampNitrous = FMath::Clamp(NitrousLevel, 0.0f, 100.0f);
		NitrousLevel = clampNitrous;
	}
}

float AGp4_Race_FinalPawn::GetNitrousLevel()
{
	return NitrousLevel;
}

void AGp4_Race_FinalPawn::Steering(const FInputActionValue& Value)
{
	// get the input magnitude for steering
	float SteeringValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetSteeringInput(SteeringValue);
}

void AGp4_Race_FinalPawn::Throttle(const FInputActionValue& Value)
{
	// get the input magnitude for the throttle
	float ThrottleValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetThrottleInput(ThrottleValue);
}

void AGp4_Race_FinalPawn::Brake(const FInputActionValue& Value)
{
	// get the input magnitude for the brakes
	float BreakValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetBrakeInput(BreakValue);
}

void AGp4_Race_FinalPawn::StartBrake(const FInputActionValue& Value)
{
	// call the Blueprint hook for the break lights
	BrakeLights(true);
}

void AGp4_Race_FinalPawn::StopBrake(const FInputActionValue& Value)
{
	// call the Blueprint hook for the break lights
	BrakeLights(false);

	// reset brake input to zero
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void AGp4_Race_FinalPawn::StartHandbrake(const FInputActionValue& Value)
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(true);

	// call the Blueprint hook for the break lights
	BrakeLights(true);
}

void AGp4_Race_FinalPawn::StopHandbrake(const FInputActionValue& Value)
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(false);

	// call the Blueprint hook for the break lights
	BrakeLights(false);
}

void AGp4_Race_FinalPawn::LookAround(const FInputActionValue& Value)
{
	// get the flat angle value for the input 
	float LookValue = Value.Get<float>();

	// add the input
	BackSpringArm->AddLocalRotation(FRotator(0.0f, LookValue, 0.0f));
}

void AGp4_Race_FinalPawn::ToggleCamera(const FInputActionValue& Value)
{
	// toggle the active camera flag
	bFrontCameraActive = !bFrontCameraActive;

	FrontCamera->SetActive(bFrontCameraActive);
	BackCamera->SetActive(!bFrontCameraActive);
}

void AGp4_Race_FinalPawn::ResetVehicle(const FInputActionValue& Value)
{
	// reset to a location slightly above our current one
	FVector ResetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	// reset to our yaw. Ignore pitch and roll
	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.0f;
	ResetRotation.Roll = 0.0f;
	
	// teleport the actor to the reset spot and reset physics
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);

	UE_LOG(LogTemplateVehicle, Error, TEXT("Reset Vehicle"));
}

#undef LOCTEXT_NAMESPACE