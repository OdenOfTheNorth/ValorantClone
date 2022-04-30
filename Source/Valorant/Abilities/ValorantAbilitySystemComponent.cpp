// Fill out your copyright notice in the Description page of Project Settings.


#include "Valorant/Abilities/ValorantAbilitySystemComponent.h"

UValorantAbilitySystemComponent::UValorantAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: UAbilitySystemComponent(ObjectInitializer)
{
}

bool UValorantAbilitySystemComponent::GetShouldTick() const
{
	return Super::GetShouldTick();
}

void UValorantAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
