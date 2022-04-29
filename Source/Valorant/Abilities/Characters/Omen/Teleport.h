// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Valorant/Abilities/Ability.h"
#include "Teleport.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UTeleport : public UAbility
{
	GENERATED_BODY()
public:
	void StartUsing() override;

	virtual void SetTeleportPosition();

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerStartTeleport();
	virtual void StartTeleport();
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerSetPosition();
	virtual void SetPosition();

	FVector TeleportPos;
	//UPROPERTY(Replicated ,EditDefaultsOnly)
	float Range = 1000;
	//UPROPERTY(Replicated ,EditDefaultsOnly)
	//bool AbilitySelected = false;
	float IsTeleporting = false;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TeleportTime = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* TeleportEffect;
	
	FTimerHandle TimerHandle_TeleportTime;

};


