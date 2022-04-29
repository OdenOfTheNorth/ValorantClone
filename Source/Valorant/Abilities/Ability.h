// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valorant/Weapon/Hands.h"
#include "Ability.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UAbility : public UHands
{
	GENERATED_BODY()
public:
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void StartUsingServer() override;
	virtual void StartUsing() override;
		
	virtual void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	AActor* MyOwner;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	int Uses = 3;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	bool AbilitySelected = true;
};
