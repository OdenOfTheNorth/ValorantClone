// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Valorant/Valorant.h"
#include "ValorantGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UValorantGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UValorantGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;	
};


