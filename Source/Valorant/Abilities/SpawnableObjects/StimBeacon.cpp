// Fill out your copyright notice in the Description page of Project Settings.


#include "Valorant/Abilities/SpawnableObjects/StimBeacon.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Valorant/Player/PlayerCharacter.h"
#include "Valorant/Weapon/PrimaryWeapon.h"


// Sets default values
AStimBeacon::AStimBeacon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStimBeacon::HandleBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AStimBeacon::HandleEndOverlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AStimBeacon::RestorFireRate()
{
	float time = GetWorld()->TimeSeconds;
	
	for (int i = 0; i < Players.Num(); i++)
	{
		if (Players[i].InBeacon)
		{
			continue;
		}
		
		if (BoostTime < time - Players[i].LeftTime)
		{
			Players[i].PrimaryWeapon->RateOfFire = Players[i].WeaponsDefaultFireRate[0];
			Players[i].SecondaryWeapon->RateOfFire = Players[i].WeaponsDefaultFireRate[1];
			Players[i].Knife->RateOfFire = Players[i].WeaponsDefaultFireRate[2];
	
			Players.RemoveAt(i);
		}
	}
}

void AStimBeacon::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerCharacter)
	{
		return;	
	}

	bool addToList = true;
	
	for (int i = 0; i < Players.Num(); i++)
	{
		if (Players[i].Player == PlayerCharacter)
		{
			addToList = false;
			break;
		}		
	}

	if (addToList)
	{
		FWeaponBoost ToAdd;

		ToAdd.Player = PlayerCharacter;
	
		ToAdd.WeaponsDefaultFireRate[0] = PlayerCharacter->PrimaryWeapon->RateOfFire;
		ToAdd.WeaponsDefaultFireRate[1] = PlayerCharacter->SecondaryWeapon->RateOfFire;
		ToAdd.WeaponsDefaultFireRate[2] = PlayerCharacter->Knife->RateOfFire;

		ToAdd.PrimaryWeapon = PlayerCharacter->PrimaryWeapon;
		ToAdd.SecondaryWeapon = PlayerCharacter->SecondaryWeapon;
		ToAdd.Knife = PlayerCharacter->Knife;
		ToAdd.InBeacon = true;
		
		Players.Add(ToAdd);
	}	

	PlayerCharacter->PrimaryWeapon->RateOfFire *= increaseFireRate;
	PlayerCharacter->SecondaryWeapon->RateOfFire *= increaseFireRate;
	PlayerCharacter->Knife->RateOfFire *= increaseFireRate;
}

void AStimBeacon::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerCharacter)
	{
		return;	
	}
	
	for (int i = 0; i < Players.Num(); i++)
	{
		if (Players[i].Player == PlayerCharacter)
		{
			Players[i].InBeacon = false;
			Players[i].LeftTime = GetWorld()->TimeSeconds;
		}		
	}
}

// Called when the game starts or when spawned
void AStimBeacon::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AStimBeacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RestorFireRate();
}

