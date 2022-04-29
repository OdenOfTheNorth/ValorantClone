// Fill out your copyright notice in the Description page of Project Settings.


#include "Valorant/Abilities/SpawnableObjects/Turret.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent      = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	StaticMeshComponent     = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	HeadSceneComponent      = CreateDefaultSubobject<USceneComponent>(TEXT("HeadSceneComponent"));
	HeadStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadStaticMeshComponent"));
	
	RootComponent = RootSceneComponent;
	StaticMeshComponent->		SetupAttachment(RootSceneComponent);     
	HeadSceneComponent->		SetupAttachment(StaticMeshComponent);      
	HeadStaticMeshComponent->	SetupAttachment(HeadSceneComponent); 
	
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

