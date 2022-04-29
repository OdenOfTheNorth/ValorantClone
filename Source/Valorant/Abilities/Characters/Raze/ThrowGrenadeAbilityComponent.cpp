// Fill out your copyright notice in the Description page of Project Settings.


#include "Valorant/Abilities/Characters/Raze/ThrowGrenadeAbilityComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Valorant/Abilities/SpawnableObjects/RazeGrenade.h"

void UThrowGrenadeAbilityComponent::Start()
{
	if (!MyOwner->HasAuthority())
	{
		ServerStart();
	}
	
	if (!AbilitySelected)
	{
		AbilitySelected = true;
	}
	else
	{
		ThrowGrenade();
	}
}

void UThrowGrenadeAbilityComponent::ServerStart_Implementation()
{
	Start();
}

bool UThrowGrenadeAbilityComponent::ServerStart_Validate()
{
	return true;
}

void UThrowGrenadeAbilityComponent::ServerThrowGrenade_Implementation()
{
	ThrowGrenade();
}

bool UThrowGrenadeAbilityComponent::ServerThrowGrenade_Validate()
{
	return true;
}

void UThrowGrenadeAbilityComponent::ThrowGrenade()
{
	if (!MyOwner->HasAuthority())
	{
		ServerThrowGrenade();
	}
	
	AbilitySelected = false;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UE_LOG(LogTemp, Warning, TEXT("ThrowGrenade"));

	FVector StartLocation;
	FRotator Rotation;
	
	MyOwner->GetActorEyesViewPoint(StartLocation, Rotation);

	FVector Dir = Rotation.Vector();
	FVector SpawnPos = StartLocation + (Dir * 100);
	
	ARazeGrenade* GrenadePTR = GetWorld()->SpawnActor<ARazeGrenade>(Grenade, SpawnPos,FRotator::ZeroRotator, SpawnParams);

	if (GrenadePTR)
	{
		GrenadePTR->ProjectileMovementComponent->Velocity = Dir * 10000;
	}

}
