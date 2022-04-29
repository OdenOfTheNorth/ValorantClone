// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VisionSensingSettings.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UVisionSensingSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Sensing)
	float Angle = 180.0f;

	UPROPERTY(EditAnywhere, Category = Sensing)
	float DistanceMinimum = 8000.0f;
};
