#include "Hands.h"
/*
void UHands::StartUsingServer_Implementation()
{
}

bool UHands::StartUsingServer_Validate()
{
}

void UHands::StopUsingServer_Implementation()
{
}

bool UHands::StopUsingServer_Validate()
{
}*/

void UHands::StartUsingServer()
{
}

void UHands::StopUsingServer()
{
}

void UHands::StartUsing()
{
}

void UHands::StopUsing()
{
}

bool UHands::IsTickable() const
{
	return FTickableGameObject::IsTickable();
}

bool UHands::IsTickableInEditor() const
{
	return FTickableGameObject::IsTickableInEditor();
}

bool UHands::IsTickableWhenPaused() const
{
	return FTickableGameObject::IsTickableWhenPaused();
}

TStatId UHands::GetStatId() const
{
	return TStatId();
}

UWorld* UHands::GetWorld() const
{
	return UObject::GetWorld();
}

void UHands::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

}

void UHands::Tick(float DeltaTime)
{

}

