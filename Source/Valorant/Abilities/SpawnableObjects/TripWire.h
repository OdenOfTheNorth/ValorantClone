// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Valorant/HealthComponent.h"
#include "TripWire.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class VALORANT_API ATripWire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATripWire();

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StartMesh;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* EndMesh;
	UPROPERTY(EditDefaultsOnly)
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	TSubclassOf<UDamageType> TripWireDamageType;

	FVector StartLocation;
	FVector ShotDirection;	
	FVector TraceEnd; 

	UPROPERTY(EditDefaultsOnly)
	float Range = 600;

	bool Activated = true;

	AActor* TrapedActor;
	AActor* MyOwner;

	UFUNCTION(Server, Reliable, WithValidation)
	void OnServerTrippwireActivated();
	UFUNCTION(Server, Reliable, WithValidation)
	void OnServerTrippwireDeactivated();

	void OnTrippwireActivated();
	void OnTrippwireDeactivated();

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health,	float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	float Damage = 5;
	FTimerHandle TimeHandler_TripWire;
	float TrappedTime = 3.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
