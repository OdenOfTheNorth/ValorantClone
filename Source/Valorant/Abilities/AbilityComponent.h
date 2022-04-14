// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilityComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

	UFUNCTION(BlueprintCallable)
	virtual void Start();
	UFUNCTION(BlueprintCallable)
	virtual void End();
	UFUNCTION(BlueprintCallable)
	virtual void StartDeploy();
	UFUNCTION(BlueprintCallable)
	virtual void EndDeploy();
	/*
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerStart();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerEnd();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerStartDeploy();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerEndDeploy();*/

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	int Uses = 1;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	bool AbilitySelected = true;

	UPROPERTY(Replicated)
	bool AbilityInput = false;

	AActor* MyOwner;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
