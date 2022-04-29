// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VisionSensingTargetComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class VALORANT_API UVisionSensingTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	FVector GetTargetOrigin() const;
	
	static void GetSensingTargets(TArray<UVisionSensingTargetComponent*>& OutTargets, const FVector& Origin, float DistanceMin);
	static TArray<UVisionSensingTargetComponent*> TargetsList;
};
