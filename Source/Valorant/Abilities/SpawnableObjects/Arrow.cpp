// Fill out your copyright notice in the Description page of Project Settings.


#include "Valorant/Abilities/SpawnableObjects/Arrow.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
}

void AArrow::ServerExploded_Implementation()
{
	Explode();
}

bool AArrow::ServerExploded_Validate()
{
	return true;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArrow::Explode()
{
	if (!HasAuthority())
	{
		ServerExploded();
	}
	
	TArray<AActor*> OverlappedActors;
	//UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), BlastRadius,
	//	ExploadeOverlapObjectTypes, AActor::StaticClass(), TArray<AActor*>(),OverlappedActors);

	UGameplayStatics::ApplyRadialDamage(this, Damage, GetActorLocation(),
		BlastRadius, nullptr, OverlappedActors, this, GetInstigatorController(), true);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImapctEffect, GetActorLocation(), GetActorRotation());

	Destroy();
}

void AArrow::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{	
	if (CurrentBounce < MaxBounces)
	{
		Velocity = FMath::GetReflectionVector(Velocity, SweepHit.Normal);
		return;
	}	

	Explode();
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector GravityAcceleration = FVector::UpVector * -GravityStrength;

	Velocity += (Acceleration + GravityAcceleration) * DeltaTime;

	FVector DeltaToMove = Velocity * DeltaTime;	

	const int MaxIterations = 5;
	int IterationsCount = 0;

	//Collision
	while (!DeltaToMove.IsNearlyZero() && ++IterationsCount < MaxIterations)
	{

		FHitResult Hit;
		AddActorWorldOffset(DeltaToMove, true, &Hit);
		DeltaToMove -= DeltaToMove * Hit.Time;

		if (Hit.bBlockingHit)
		{
			if (Hit.bStartPenetrating)
			{
				FVector DepenVector = Hit.Normal * (Hit.PenetrationDepth + 0.01f);
				AddActorWorldOffset(DepenVector);
				continue;
			}

			FVector ImapctVelocity = FVector::DotProduct(Velocity, Hit.Normal) * Hit.Normal;
			Velocity -= ImapctVelocity;
			DeltaToMove -= FVector::DotProduct(DeltaToMove, Hit.Normal) * Hit.Normal;
		}
	}

	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(Velocity.GetSafeNormal());
	SetActorRotation(Rotation);

}

