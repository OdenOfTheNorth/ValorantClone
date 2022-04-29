// Fill out your copyright notice in the Description page of Project Settings.

#include "Valorant/Abilities/Characters/Omen/Teleport.h"

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Valorant/Player/PlayerCharacter.h"

void UTeleport::ServerStartTeleport_Implementation()
{
	StartTeleport();
}

bool UTeleport::ServerStartTeleport_Validate()
{
	return true;
}

void UTeleport::StartUsing()
{	
	AbilitySelected = true;
	StartTeleport();
}

void UTeleport::SetTeleportPosition()
{/*
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

		FTimerManager& Manager = GetWorld()->GetTimerManager();
		
		//Manager.SetTimer(TimerHandle_TeleportTime, this,
		//	&UTeleport::SetTeleportPosition,0,  AbilityIsSelected);
	}*/
}

void UTeleport::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
	
	if (Uses <= 0 || !AbilitySelected)
	{
		//return;
	}
	
	if (MyOwner)
	{
		GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, TEXT("MyOwner"));
		
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

		//bool IsLocallyControlledCharacter = ;
		
		//Draw

		APawn* Pawn = Cast<APawn>(MyOwner);

		if (Pawn && Pawn->GetController() && Pawn->GetController()->IsLocalPlayerController()) 
		{
			//DrawDebugLine(GetWorld(), StartLocation, HitLocation, FColor::White, false, 1.f, 0.f, 1.f);
			//DrawDebugLine(GetWorld(), HitLocation, HitLocationFloor, FColor::White, false, 1.f, 0.f, 1.f);
			DrawDebugSphere(GetWorld(), HitLocationFloor, 50, 16, FColor::Red);

			if (TeleportEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, TeleportPos);
			}
		}

	}else
	{
		GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, TEXT("no owner"));
	}
}

void UTeleport::StartTeleport()
{
	if (!MyOwner->HasAuthority())
	{
		ServerStartTeleport();
	}

	IsTeleporting = true;
	
	FTimerManager& Manager = GetWorld()->GetTimerManager();
	
	Manager.SetTimer(TimerHandle_TeleportTime, this, &UTeleport::SetPosition,
		TeleportTime,  false);

	if (TeleportEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, MyOwner->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, TeleportPos);	
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(MyOwner);
	Player->CanMove = false;
	//Player->GetMovementComponent()->Deactivate();
}

void UTeleport::ServerSetPosition_Implementation()
{
}

bool UTeleport::ServerSetPosition_Validate()
{
	return true;
}

void UTeleport::SetPosition()
{
	if (!MyOwner->HasAuthority())
	{
		ServerSetPosition();
	}
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(MyOwner);
	Player->CanMove = true;
	//Player->GetMovementComponent()->Activate();
	
	float Height = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	Player->SetActorLocation(TeleportPos + FVector::UpVector * Height);
	
	UE_LOG(LogTemp,Warning,TEXT("ServerEndDeploy"));
	//AbilitySelected = false;
	IsTeleporting = false;
}