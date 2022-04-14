// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CypherCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;

UCLASS()
class VALORANT_API ACypherCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACypherCamera();

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
