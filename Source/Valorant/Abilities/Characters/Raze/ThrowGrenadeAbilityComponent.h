// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valorant/Abilities/AbilityComponent.h"
#include "ThrowGrenadeAbilityComponent.generated.h"

class ARazeGrenade;
/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UThrowGrenadeAbilityComponent : public UAbilityComponent
{
	GENERATED_BODY()
public:
	virtual void Start() override;

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerStart();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerThrowGrenade();
	void ThrowGrenade();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARazeGrenade> Grenade;
	int Grenades = 2;	
};
