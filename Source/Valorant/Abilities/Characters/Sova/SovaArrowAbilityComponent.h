// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valorant/Abilities/AbilityComponent.h"
#include "SovaArrowAbilityComponent.generated.h"


class AArrow;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API USovaArrowAbilityComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USovaArrowAbilityComponent();
	
	float Force = 0;
	float MaxForce = 1000;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Start() override;
	virtual void End() override;
	virtual void StartDeploy() override;
	virtual void EndDeploy() override;

	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerStart();
	//UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	//virtual void ServerEnd();
	//UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	//virtual void ServerStartDeploy();
	//UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	//virtual void ServerEndDeploy();
	

	bool LastFrameAbilityInput = false;
	bool CanShoot = false;
	float ForceBuildUp = 300;


	int Arrows = 0;
	int MaxArrows = 2;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AArrow> Arrow;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

		
};
