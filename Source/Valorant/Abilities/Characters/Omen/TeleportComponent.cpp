// Fill out your copyright notice in the Description page of Project Settings.

#include "Valorant/Abilities/Characters/Omen/TeleportComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Valorant/Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTeleportComponent::UTeleportComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTeleportComponent::Start()
{
	if (!MyOwner->HasAuthority())
	{
		ServerStart();
	}

	if (IsTeleporting)
	{
		return;
	}
	
	if (!AbilitySelected)
	{
		AbilitySelected = true;
	}
	else
	{
		//AbilitySelected = false;
		StartTeleport();
	}
}
/*
void UTeleportComponent::End()
{
	if (!MyOwner->HasAuthority())
	{
		//ServerEnd();
	}

	AbilitySelected = false;
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	Player->CanMove = true;
}

void UTeleportComponent::StartDeploy()
{
	if (!MyOwner->HasAuthority())
	{
		//ServerStartDeploy();
	}
	
	AbilitySelected = false;
	
	FTimerManager& Manager = GetWorld()->GetTimerManager();
	
	Manager.SetTimer(TimerHandle_TeleportTime, this, &UTeleportComponent::SetPosition,
		TeleportTime,  false);

	if (TeleportEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, GetOwner()->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, TeleportPos);	
	}	

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	Player->CanMove = false;
	//Player->GetMovementComponent()->Deactivate();
}

void UTeleportComponent::EndDeploy()
{
	Super::EndDeploy();
}*/

void UTeleportComponent::ServerStart_Implementation()
{
	Start();
}

bool UTeleportComponent::ServerStart_Validate()
{
	return true;
}

void UTeleportComponent::BeginPlay()
{
	Super::BeginPlay();

	AbilitySelected = false;
	MyOwner = GetOwner();
}

void UTeleportComponent::ServerSetPosition_Implementation()
{
	SetPosition();
}

bool UTeleportComponent::ServerSetPosition_Validate()
{
	return true;
}

void UTeleportComponent::ServerStartTeleport_Implementation()
{
	StartTeleport();
}

bool UTeleportComponent::ServerStartTeleport_Validate()
{
	return true;
}

void UTeleportComponent::StartTeleport()
{
	if (!MyOwner->HasAuthority())
	{
		ServerStartTeleport();
	}

	IsTeleporting = true;
	AbilitySelected = false;
	
	FTimerManager& Manager = GetWorld()->GetTimerManager();
	
	Manager.SetTimer(TimerHandle_TeleportTime, this, &UTeleportComponent::SetPosition,
		TeleportTime,  false);

	if (TeleportEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, GetOwner()->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, TeleportPos);	
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	Player->CanMove = false;
	//Player->GetMovementComponent()->Deactivate();
}

void UTeleportComponent::SetPosition()
{
	if (!MyOwner->HasAuthority())
	{
		ServerSetPosition();
	}
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	Player->CanMove = true;
	//Player->GetMovementComponent()->Activate();
	
	float Height = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	Player->SetActorLocation(TeleportPos + FVector::UpVector * Height);
	
	UE_LOG(LogTemp,Warning,TEXT("ServerEndDeploy"));
	AbilitySelected = false;
	IsTeleporting = false;
	//ServerEnd();
}


// Called every frame
void UTeleportComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Uses <= 0 || !AbilitySelected)
	{
		return;
	}
	
	MyOwner = GetOwner();

	if (MyOwner)
	{
		FVector StartLocation;
		FRotator Rotation;

		MyOwner->GetActorEyesViewPoint(StartLocation, Rotation);

		FVector ShotDirection = Rotation.Vector();		
		FVector TraceEnd = StartLocation + (ShotDirection * Range);
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);

		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		//First Hit
		FHitResult Hit;
		FVector HitLocation = TraceEnd;	
		
		GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, HitLocation, ECC_WorldDynamic , QueryParams);
		
		if (Hit.bBlockingHit)
		{
			HitLocation = Hit.ImpactPoint + (Hit.ImpactNormal * 100);
		}		

		//Second Hit
		FHitResult HitGround;
		FVector HitLocationFloor = HitLocation  + (FVector::DownVector * 10000);		
	
		GetWorld()->LineTraceSingleByChannel(HitGround, HitLocation, HitLocationFloor, ECC_WorldDynamic, QueryParams);
		
		if (HitGround.bBlockingHit)
		{
			HitLocationFloor = HitGround.ImpactPoint;
			TeleportPos = HitLocationFloor;
		}

		//Draw
		if (MyOwner->GetInstigatorController()->IsLocalController())
		{
			DrawDebugLine(GetWorld(), StartLocation, HitLocation, FColor::White, false, 1.f,0.f,1.f);
			DrawDebugLine(GetWorld(), HitLocation, HitLocationFloor, FColor::White, false, 1.f,0.f,1.f);
			DrawDebugSphere(GetWorld(), HitLocationFloor, 50, 16, FColor::Red);
		}

		if (true)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, TeleportPos);	
		}
	}
}

void UTeleportComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilityComponent, AbilitySelected);
	DOREPLIFETIME(UAbilityComponent, Uses);	
}
