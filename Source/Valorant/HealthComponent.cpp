// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//SetReplicates(true);
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	// Only ad on damage event if we are on server
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();

		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnHandleAnyDamage);
		}
	}
}

void UHealthComponent::OnHandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	//Clamp Health
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	UE_LOG(LogGameMode, Log, TEXT("Damage %s , Health %s"), *FString::SanitizeFloat(Damage) ,*FString::SanitizeFloat(CurrentHealth));

	OnHealthChanged.Broadcast(this, CurrentHealth,Damage, DamageType, InstigatedBy, DamageCauser);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

