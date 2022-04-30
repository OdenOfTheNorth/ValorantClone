// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ValorantAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UValorantAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UValorantAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	//bool bCharacterAbilitiesGiven = false;
	//bool bStartupEffectsApplied = false;

	virtual bool GetShouldTick() const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
