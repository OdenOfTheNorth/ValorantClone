// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Valorant/Weapon/BasicWeapon.h"
#include "Valorant/Weapon/KnifeSlot.h"
#include "Valorant/Weapon/PrimaryWeapon.h"
#include "Valorant/Weapon/SecondaryWeapon.h"
#include "StimBeacon.generated.h"

USTRUCT()
struct FWeaponBoost
{
	GENERATED_BODY()
	APlayerCharacter* Player;
	APrimaryWeapon* PrimaryWeapon;
	ASecondaryWeapon* SecondaryWeapon;
	AKnifeSlot* Knife;
	TArray<float> WeaponsDefaultFireRate;

	float LeftTime = 0;
	bool InBeacon = false;
};

UCLASS()
class VALORANT_API AStimBeacon : public AActor
{
	GENERATED_BODY()
	
public:	
	AStimBeacon();

	UStaticMeshComponent* StaticMeshComponent;
	UBoxComponent* BoxComponent;

	TArray<FWeaponBoost> Players;

	float increaseFireRate = 0.2f;

	float BoostTime = 5;
	
	void RestorFireRate();

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit);
	UFUNCTION()
	void HandleEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
