// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraObject.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;

UCLASS()
class VALORANT_API ACameraObject : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraObject();

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMesh = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh = nullptr;

	APawn* OwnerPawn;

	UPROPERTY(EditDefaultsOnly)
	bool CanMove = true;
	UPROPERTY(EditDefaultsOnly)
	bool CanLook = true;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void OnPossess();
	void UnPossess();
	
	void HandleMoveForward(float Value);
	void HandleMoveRight(float Value);
	void HandleMoveUp(float Value);

	void HandleLookRight(float Value);
	void HandleLookUp(float Value);

	FVector SetMoveDirection();
	//bool CheckDroneHeight();
	void Move(float DeltaTime);

	FVector RightCameraVector;
	FVector ForwardCameraVector;
	
	FVector Velocity;
	FVector MoveDirection = FVector::ZeroVector;
	
	//UPROPERTY(EditDefaultsOnly)
	//float MaxHeight = 500;
	
	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed = 300;
	
	//UPROPERTY(EditDefaultsOnly)
	//float TurnSpeed = 250;
	
	float RightInput;
	float ForwardInput;
	float UpInput;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
