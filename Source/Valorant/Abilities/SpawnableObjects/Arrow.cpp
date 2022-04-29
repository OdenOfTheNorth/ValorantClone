#include "Valorant/Abilities/SpawnableObjects/Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
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
		//ProjectileMovementComponent->bShouldBounce = true;

		FVector d = ProjectileMovementComponent->Velocity;
		FVector n = SweepHit.ImpactNormal;
		FVector RD = d - 2 * (d * n) * n;
		
		ProjectileMovementComponent->Velocity = RD;
		
		CurrentBounce++;
		return;
	}	

	Explode();
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(ProjectileMovementComponent->Velocity.GetSafeNormal());
	SetActorRotation(Rotation);
}

