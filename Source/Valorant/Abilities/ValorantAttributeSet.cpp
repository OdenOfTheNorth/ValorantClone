// Fill out your copyright notice in the Description page of Project Settings.


#include "Valorant/Abilities/ValorantAttributeSet.h"
#include "Net/UnrealNetwork.h"

UValorantAttributeSet::UValorantAttributeSet()
{
}

void UValorantAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UValorantAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UValorantAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UValorantAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UValorantAttributeSet, PrimaryAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UValorantAttributeSet, PrimaryCooldown, COND_None, REPNOTIFY_Always);
}

void UValorantAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UValorantAttributeSet, Health, OldHealth);
}

void UValorantAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UValorantAttributeSet, Shield, OldShield);
}

void UValorantAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UValorantAttributeSet, AttackPower, OldAttackPower);
}

void UValorantAttributeSet::OnRep_PrimaryAmmo(const FGameplayAttributeData& OldPrimaryAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UValorantAttributeSet, PrimaryAmmo, OldPrimaryAmmo);
}

void UValorantAttributeSet::OnRep_PrimaryCooldown(const FGameplayAttributeData& OldPrimaryCooldown)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UValorantAttributeSet, PrimaryCooldown, OldPrimaryCooldown);
}
