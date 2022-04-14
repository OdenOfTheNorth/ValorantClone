// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Valorant/Valorant.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Valorant/Player/PlayerCharacter.h"

// Sets default values
ABasicWeapon::ABasicWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletonMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = SkeletonMeshComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(StaticMeshComp);
	
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	SetReplicates(true);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void ABasicWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire; 
	CurrentAmmo = MaxAmmo;

	Player = Cast<APlayerCharacter>(GetOwner());
}

void ABasicWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ABasicWeapon::ServerFire_Validate()
{
	return true;
}

void ABasicWeapon::Fire()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dose Not Have Authority"));
		ServerFire();		
	}	
	
	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{				
		if (CurrentAmmo <= 0 || !CanShot)
		{
			return;
		}
		
		FVector StartLocation;
		FRotator Rotation;

		MyOwner->GetActorEyesViewPoint(StartLocation, Rotation);

		FVector ShotDirection = Rotation.Vector();		
		FVector TraceEnd = StartLocation + (ShotDirection * 10000);
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);

		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		FVector TracerEndPoint = TraceEnd;

		UE_LOG(LogTemp, Warning, TEXT("Shooting"));
		
		if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();
			APlayerCharacter* HitPlayer = Cast<APlayerCharacter>(HitActor);

			Player = Cast<APlayerCharacter>(MyOwner);

			if (HitPlayer)
			{
				if (HitPlayer->Team != Player->Team)
				{
					SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

					FVector distVector = Hit.ImpactPoint - StartLocation;
			
					float ActualDamage = Damage;
			
					if (FallOfRange < distVector.Size())
					{
						ActualDamage *= FallOfMultiplier; 
					}			

					if (SurfaceType == SURFACE_FLESHCRITICAL)
					{
						ActualDamage *= HeadShotMultiplier;
					}
				
					UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);	
				}	
			}		
			
			PlaySurfaceEffect(Hit.ImpactPoint, SurfaceType);

			TracerEndPoint = Hit.ImpactPoint;
		}

		//DrawDebugLine(GetWorld(), StartLocation, TraceEnd, FColor::White, false, 1.f,0.f,1.f);

		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, StartLocation);	
		}	
		
		PlayFireEffects(TracerEndPoint);

		LastFireTime = GetWorld()->TimeSeconds;

		CurrentAmmo--;

		//UE_LOG(LogTemp, Warning, TEXT("Current ammo %s, / Max Ammo %s"),CurrentAmmo, MaxAmmo);

		if (HasAuthority())
		{
			HitScanTrace.TraceEnd = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}
	}
}


void ABasicWeapon::ServerReload_Implementation()
{
	StartReload();
}

bool ABasicWeapon::ServerReload_Validate()
{
	return true;
}

void ABasicWeapon::StartReload()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dose Not Have Authority"));
		ServerReload();		
	}
	
	UE_LOG(LogTemp, Warning, TEXT("StartReload"));
	CanShot = false;

	GetWorldTimerManager().SetTimer(TimerHandle_ReloadTime, this, &ABasicWeapon::EndReload, ReloadTime, false);

	//Player = Cast<APlayerCharacter>(GetOwner());
	//FTimerManager Manager;
	//Manager.SetTimer(TimerHandle_ReloadTime, this, &ABasicWeapon::EndReload, ReloadTime, false);
	//Player->GetWorldTimerManager().SetTimer(TimerHandle_ReloadTime, this, &ABasicWeapon::EndReload, ReloadTime, false);
}

void ABasicWeapon::EndReload()
{
	UE_LOG(LogTemp, Warning, TEXT("EndReload"));
	CanShot = true;
	CurrentAmmo = MaxAmmo;
	GetWorldTimerManager().ClearTimer(TimerHandle_ReloadTime);	
}

void ABasicWeapon::StartSecondary()
{
	if (CanADS)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartSecondary"));
		WantToADS = true;
	}
}

void ABasicWeapon::EndSecondary()
{
	if (CanADS)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndSecondary"));
		WantToADS = false;
	}
}

void ABasicWeapon::OnRep_HitScanTrace()
{
	PlayFireEffects(HitScanTrace.TraceEnd);
	PlaySurfaceEffect(HitScanTrace.TraceEnd,HitScanTrace.SurfaceType);
}

void ABasicWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABasicWeapon, HitScanTrace,COND_SkipOwner);
}

// Called every frame
void ABasicWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanADS && Player)
	{
		float TargetFOV = WantToADS ? ADS_FOV : Player->DefaultFOV;
		float NewFOV = FMath::FInterpTo(Player->CameraComp->FieldOfView, TargetFOV, DeltaTime, ADS_Speed);

		bool PlayerExist = false;

		if (Player)
		{
			PlayerExist = true;
		}
		
		//UE_LOG(LogTemp, Warning, TEXT("NewFOV %f , WantToADS %d, Player %d"), NewFOV, WantToADS, PlayerExist);
		
		Player->CameraComp->SetFieldOfView(NewFOV);
	}
}

void ABasicWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ABasicWeapon::Fire, TimeBetweenShots, Automatic, FirstDelay);
}

void ABasicWeapon::EndFire()
{	
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);	
}

void ABasicWeapon::PlayFireEffects(FVector TraceEnd)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SkeletonMeshComp, MuzzleSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no MuzzleEffect"));
	}

	if (TracerEffect)
	{
		FVector MuzzlePos = MuzzleLocation->GetComponentLocation();

		if (SkeletonMeshComp)
		{
			MuzzlePos = SkeletonMeshComp->GetSocketLocation(MuzzleSocketName);
		}

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzlePos);

		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no TracerEffect"));
	}
}

void ABasicWeapon::PlaySurfaceEffect(FVector ImpactPoint, EPhysicalSurface SurfaceType)
{
	UParticleSystem* SelectedEffect = nullptr;
			
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:// break removed go to case 2
	case SURFACE_FLESHCRITICAL:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = ImpactEffect;
		break;
	}
			
	if (SelectedEffect)
	{
		FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
		
		if (SkeletonMeshComp)
		{
			MuzzlePos = SkeletonMeshComp->GetSocketLocation(MuzzleSocketName);
		}

		FVector ShotDirection = ImpactPoint - MuzzlePos;
		ShotDirection.Normalize();
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no ImpactEffect"));
	}
}

