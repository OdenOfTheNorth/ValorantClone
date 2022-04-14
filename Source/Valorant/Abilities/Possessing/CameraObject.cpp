// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraObject.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/SpringArmComponent.h"

ACameraObject::ACameraObject()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(SceneComponent);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(SceneComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void ACameraObject::BeginPlay()
{
	Super::BeginPlay();
	//OwnerPawn = Cast<APawn>(GetOwner());
}

void ACameraObject::UnPossess()
{
	AController* TempController = GetInstigatorController();
	
	if (OwnerPawn)
	{
		TempController->Possess(OwnerPawn);
		UE_LOG(LogTemp,Warning, TEXT("Posses"))
	}
	else
	{
		UE_LOG(LogTemp,Warning, TEXT("ACameraObject no OwnerPawn"))
	}	
}

void ACameraObject::HandleMoveForward(float Value)
{
	ForwardInput = Value;
}

void ACameraObject::HandleMoveRight(float Value)
{
	RightInput = Value;
}

void ACameraObject::HandleMoveUp(float Value)
{
	ForwardInput = Value;
}

void ACameraObject::HandleLookRight(float Value)
{
	if (!CanLook)
	{
		return;
	}

	AddControllerYawInput(Value);
}

void ACameraObject::HandleLookUp(float Value)
{
	if (!CanLook)
	{
		return;
	}

	AddControllerPitchInput(Value);
}

FVector ACameraObject::SetMoveDirection()
{
	ForwardCameraVector = FVector::VectorPlaneProject(CameraComponent->GetForwardVector(),FVector::UpVector);
	RightCameraVector = FVector::CrossProduct(-ForwardCameraVector, FVector::UpVector);
	
	MoveDirection = (ForwardCameraVector * ForwardInput) + (RightCameraVector * RightInput) + (FVector::UpVector * UpInput);

	return MoveDirection.GetSafeNormal();
}

void ACameraObject::Move(float DeltaTime)
{
	if (!CanMove)
	{
		return;
	}
	
	Velocity += SetMoveDirection() * MoveSpeed;
	Velocity -= Velocity * 0.1f;
	FVector DeltaToMove = Velocity * DeltaTime;

	const int MaxIterations = 5;
	int IterationsCount = 0;

	FHitResult Hit;

	//Collision
	while (!DeltaToMove.IsNearlyZero() && ++IterationsCount < MaxIterations)
	{
		AddActorWorldOffset(DeltaToMove, true, &Hit);
		DeltaToMove -= DeltaToMove * Hit.Time;

		if (Hit.bBlockingHit)
		{
			if (Hit.bStartPenetrating)
			{
				FVector DepenVector = Hit.Normal * (Hit.PenetrationDepth + 0.01f);
				AddActorWorldOffset(DepenVector);
				continue;
			}

			FVector ImapctVelocity = FVector::DotProduct(Velocity, Hit.Normal) * Hit.Normal;
			Velocity -= ImapctVelocity;
			DeltaToMove -= FVector::DotProduct(DeltaToMove, Hit.Normal) * Hit.Normal;
		}
	}
}

// Called every frame
void ACameraObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

// Called to bind functionality to input
void ACameraObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACameraObject::HandleMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACameraObject::HandleMoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveUp"), this, &ACameraObject::HandleMoveUp);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACameraObject::HandleLookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACameraObject::HandleLookUp);

	PlayerInputComponent->BindAction(TEXT("EAbility"), IE_Pressed, this, &ACameraObject::UnPossess);
}

