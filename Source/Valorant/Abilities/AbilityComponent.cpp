// Fill out your copyright notice in the Description page of Project Settings.


#include "Valorant/Abilities/AbilityComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAbilityComponent::Start()
{
}

void UAbilityComponent::End()
{
}

void UAbilityComponent::StartDeploy()
{
}

void UAbilityComponent::EndDeploy()
{
}
/*
void UAbilityComponent::ServerStart_Implementation()
{
	
}

bool UAbilityComponent::ServerStart_Validate()
{
	return true;
}

void UAbilityComponent::ServerEnd_Implementation()
{
}

bool UAbilityComponent::ServerEnd_Validate()
{
	return true;
}

void UAbilityComponent::ServerStartDeploy_Implementation()
{

}

bool UAbilityComponent::ServerStartDeploy_Validate()
{
	return true;
}

void UAbilityComponent::ServerEndDeploy_Implementation()
{
}

bool UAbilityComponent::ServerEndDeploy_Validate()
{
	return true;
}*/

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = GetOwner();
	// ...
	
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilityComponent, AbilitySelected);
	DOREPLIFETIME(UAbilityComponent, Uses);	
}

/*
void UAbilityComponent::ServerStart()
{
}

void UAbilityComponent::ServerEnd()
{
}

void UAbilityComponent::ServerStartDeploy()
{
}

void UAbilityComponent::ServerEndDeploy()
{
}*/