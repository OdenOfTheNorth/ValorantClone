// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Valorant/HealthComponent.h"
#include "Valorant/Player/PlayerCharacter.h"
#include "SageWall.generated.h"

class UBoxComponent;
UCLASS()
class VALORANT_API ASageWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASageWall();

	USceneComponent* SceneComponent;
	UStaticMeshComponent* StaticMeshComponent;
	UBoxComponent* BoxComponent;

	UHealthComponent* HealthComponent;

	APlayerCharacter* MyOwner;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health,	float HealthDelta,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void SolidifyWall();
	
	float SpawnMaxHealth = 400;
	float SolidMaxHealth = 1200;

	FTimerHandle TimerHandle_TimeWallSpawn;

	float TimeToSolidifyWall = 2;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
