// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Smoke.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class VALORANT_API ASmoke : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmoke();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* SceneComponent;
	UStaticMeshComponent* StaticMeshComponent;
	UBoxComponent* BoxComponent;
	
	//UPROPERTY(EditDefaultsOnly)
	//UMaterial* 
	UPROPERTY(EditDefaultsOnly)
	float Duration = 10;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
