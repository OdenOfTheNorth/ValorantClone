// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RazeGrenade.generated.h"

class UProjectileMovementComponent;
UCLASS()
class VALORANT_API ARazeGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARazeGrenade();

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARazeGrenade> Grenade;

	UPROPERTY(EditDefaultsOnly)
	int SmallChargesAmount = 6;
	
	void Explode();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExplode();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_TimeToExpolode;
	TArray<TEnumAsByte<EObjectTypeQuery>> ExploadeOverlapObjectTypes;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImapctEffect;
	
	UPROPERTY(EditDefaultsOnly)
	float TimeToExplode = 3;

	UPROPERTY(EditDefaultsOnly)
	float BlastRadius = 3;
	
	UPROPERTY(EditDefaultsOnly)
	float Damage = 60;

	AActor* MyOwner;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
