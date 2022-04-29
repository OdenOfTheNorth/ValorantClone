#include "Valorant/Weapon/WeaponActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent        = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	StaticMeshComponent   = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
	RootComponent = SceneComponent;
	StaticMeshComponent   -> SetupAttachment(RootComponent);
	SkeletalMeshComponent -> SetupAttachment(RootComponent);
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	if (weapon->SkeletonMeshComp)
	{
		SkeletalMeshComponent->SkeletalMesh = weapon->SkeletonMeshComp;
	}

	if (weapon->StaticMeshComp)
	{
		StaticMeshComponent = weapon->StaticMeshComp;
	}
}

void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

