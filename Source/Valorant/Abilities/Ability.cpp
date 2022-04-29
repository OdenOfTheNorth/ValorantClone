#include "Valorant/Abilities/Ability.h"
#include "Net/UnrealNetwork.h"

void UAbility::StartUsingServer_Implementation()
{
	StartUsing();
}

bool UAbility::StartUsingServer_Validate()
{
	return true;
}

void UAbility::StartUsing()
{
	if (!MyOwner->HasAuthority())
	{
		StartUsingServer();
	}	
}

void UAbility::Tick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::SanitizeFloat(TestCounter));
}

bool UAbility::IsTickable() const
{
	return true;
}

bool UAbility::IsTickableInEditor() const
{
	return false;
}

bool UAbility::IsTickableWhenPaused() const
{
	return false;
}

TStatId UAbility::GetStatId() const
{
	return TStatId();
}

UWorld* UAbility::GetWorld() const
{
	return GetOuter()->GetWorld();
}

void UAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbility, AbilitySelected);
	DOREPLIFETIME(UAbility, Uses);
}
