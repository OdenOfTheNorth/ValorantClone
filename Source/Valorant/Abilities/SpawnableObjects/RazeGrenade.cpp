// Fill out your copyright notice in the Description page of Project Settings.


#include "RazeGrenade.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARazeGrenade::ARazeGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//RootComponent = SceneComponent;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	//StaticMeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	SetReplicates(true);
	//SetReplicatedMovement(true);
}

void ARazeGrenade::Explode()
{
	if (!HasAuthority())
	{
		ServerExplode();
	}

	TArray<AActor*> OverlappedActors;
	//UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), BlastRadius,
	//	ExploadeOverlapObjectTypes, AActor::StaticClass(), TArray<AActor*>(),OverlappedActors);

	UGameplayStatics::ApplyRadialDamage(this, Damage, GetActorLocation(),
		BlastRadius, nullptr, OverlappedActors, MyOwner, GetInstigatorController(), false);
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), BlastRadius, 16, FColor::Red, false, 3);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImapctEffect, GetActorLocation(), GetActorRotation());

	FVector GrenadePos = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	float Degrees = 360;

	float DegreesPerGrenade = Degrees / SmallChargesAmount;
	
	for (int i = 0; i < SmallChargesAmount; i++)
	{
		float DegreesDirection = DegreesPerGrenade * i;

		FVector Direction = FVector(FMath::Sin(FMath::DegreesToRadians(DegreesDirection)),
			FMath::Cos(FMath::DegreesToRadians(DegreesDirection)), 0);
		
		ARazeGrenade* GrenadePTR = GetWorld()->SpawnActor<ARazeGrenade>(Grenade, GrenadePos + (Direction * 10),FRotator::ZeroRotator, SpawnParams);

		if (!GrenadePTR)
		{
			continue;
		}

		GrenadePTR->ProjectileMovementComponent->Velocity = Direction.GetSafeNormal() * 150;		
	}

	Destroy();
}

void ARazeGrenade::ServerExplode_Implementation()
{
	Explode();
}

bool ARazeGrenade::ServerExplode_Validate()
{
	return true;
}


// Called when the game starts or when spawned
void ARazeGrenade::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovementComponent->bShouldBounce = true;
	GetWorldTimerManager().SetTimer(TimerHandle_TimeToExpolode, this, &ARazeGrenade::Explode, TimeToExplode, false);
}

// Called every frame
void ARazeGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

