// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h" 
#include "ValorantAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
			GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
			GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
			GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
			GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class VALORANT_API UValorantAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UValorantAttributeSet();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UValorantAttributeSet, Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UValorantAttributeSet, Shield);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UValorantAttributeSet, AttackPower);

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_PrimaryAmmo)
	FGameplayAttributeData PrimaryAmmo;
	ATTRIBUTE_ACCESSORS(UValorantAttributeSet, PrimaryAmmo);

	UFUNCTION()
	virtual void OnRep_PrimaryAmmo(const FGameplayAttributeData& OldPrimaryAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_PrimaryCooldown)
	FGameplayAttributeData PrimaryCooldown;
	ATTRIBUTE_ACCESSORS(UValorantAttributeSet, PrimaryCooldown);

	UFUNCTION()
	virtual void OnRep_PrimaryCooldown(const FGameplayAttributeData& OldPrimaryCooldown);
	
};


