// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valorant/Abilities/AbilityComponent.h"
//#include "Possessing/CameraObject.h"
#include "CypherCameraComponent.generated.h"

UENUM()
enum ECameraAction
{
	CameraSelected,
	PlaceCamera,
	PossesCamera,
	CameraPlaced,
	RemoveCamera,
	Default
};

class ACameraObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UCypherCameraComponent : public UAbilityComponent
{
	GENERATED_BODY()
public:
	UCypherCameraComponent();

	virtual void Start() override;
	virtual void End() override;
	virtual void StartDeploy() override;
	virtual void EndDeploy() override;
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerStart();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	virtual void ServerEnd();
	//UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	//virtual void ServerStartDeploy();
	//UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	//virtual void ServerEndDeploy();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerPossesCamera();
	
	void RemoveCamera();
	void PlaceCamera(FVector CamPos);
	void PossesCamera();
	void RaycastCameraSelected();

	UPROPERTY(EditDefaultsOnly)
	float Range = 1200;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACameraObject> Camera;
	
	ACameraObject* CameraObject;
	FVector CameraPos; 

	ECameraAction CameraAction;
	
	//FVector CameraPos;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
