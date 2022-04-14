// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChanged, UHealthComponent*, HealthComp, float, Health,
	float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Valorant), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float MaxHealth;
	
	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadOnly, Category = "HealthComponent")
	float CurrentHealth;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHandleAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser );

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

};
