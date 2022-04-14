// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class VALORANT_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	AArrow();

	UPROPERTY(EditDefaultsOnly)
	int MaxBounces = 2;
	int CurrentBounce = 0;	

	//UPROPERTY(Replicated)
	FVector Velocity;
	//UPROPERTY(Replicated)
	FVector Acceleration;
	float GravityStrength = 1000;
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImapctEffect;

	UPROPERTY(EditDefaultsOnly)
	float BlastRadius = 5000;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 600.f;

	UFUNCTION(unreliable, server, WithValidation)
	void ServerExploded();

	TSubclassOf<UDamageType> DamageTypes;
	TArray<TEnumAsByte<EObjectTypeQuery>> ExploadeOverlapObjectTypes;

	void Explode();

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit);

public:	
	virtual void Tick(float DeltaTime) override;

};
