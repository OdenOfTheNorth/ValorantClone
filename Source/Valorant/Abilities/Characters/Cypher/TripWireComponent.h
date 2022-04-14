// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valorant/Abilities/AbilityComponent.h"
#include "Valorant/Abilities/SpawnableObjects/TripWire.h"
#include "TripWireComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UTripWireComponent : public UAbilityComponent
{
	GENERATED_BODY()
public:
	virtual void Start() override;
	virtual void End() override;
	virtual void StartDeploy() override;
	virtual void EndDeploy() override;
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerStart();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerEnd();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerPlaceTripWire(FVector TripPos, FVector Rot);
	void PlaceTripWire(FVector TripPos, FVector Rot);
	void RemoveTripWire(ATripWire* Wire);
	
	UPROPERTY(EditDefaultsOnly)
	float Range = 1200;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATripWire> TripWire;

	void RaycastCameraSelected();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
