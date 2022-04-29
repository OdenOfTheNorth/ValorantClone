// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hands.generated.h"

UCLASS(DefaultToInstanced, BlueprintType, Blueprintable)
class VALORANT_API UHands : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	//UFUNCTION(Server, Reliable, WithValidation)
	virtual void StartUsingServer();
	UFUNCTION(BlueprintCallable)
	virtual void StartUsing();

	//UFUNCTION(Server, Reliable, WithValidation)
	virtual void StopUsingServer();
	UFUNCTION(BlueprintCallable)
	virtual void StopUsing();

	virtual void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }


	AActor* MyOwner;
};
