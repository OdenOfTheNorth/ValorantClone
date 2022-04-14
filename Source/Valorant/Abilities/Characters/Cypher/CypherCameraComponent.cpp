// Fill out your copyright notice in the Description page of Project Settings.


#include "CypherCameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Valorant/Abilities/Possessing/CameraObject.h"

UCypherCameraComponent::UCypherCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCypherCameraComponent::Start()
{
	if (!MyOwner->HasAuthority())
	{
		ServerStart();
	}	
	
	if (!AbilitySelected)
	{
		AbilitySelected = true;
	}
	else
	{
		AbilitySelected = false;
		AbilityInput = true;
	}
	
	UE_LOG(LogTemp,Warning, TEXT("ServerStart %d"), AbilitySelected);
	
}

void UCypherCameraComponent::End()
{
	if (!MyOwner->HasAuthority())
	{
		ServerEnd();
	}
	
	AbilitySelected = false;
	UE_LOG(LogTemp,Warning, TEXT("ServerEnd %d"), AbilitySelected);
}

void UCypherCameraComponent::StartDeploy()
{

}

void UCypherCameraComponent::EndDeploy()
{

}

void UCypherCameraComponent::ServerStart_Implementation()
{
	Start();	
}

bool UCypherCameraComponent::ServerStart_Validate()
{
	return true;
}

void UCypherCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	AbilitySelected = false;
	MyOwner = GetOwner();
}

void UCypherCameraComponent::ServerEnd_Implementation()
{
	End();
}

bool UCypherCameraComponent::ServerEnd_Validate()
{
	return true;
}






void UCypherCameraComponent::ServerPossesCamera_Implementation()
{
	PossesCamera();
}

bool UCypherCameraComponent::ServerPossesCamera_Validate()
{
	return true;
}

void UCypherCameraComponent::RemoveCamera()
{
	AbilityInput = false;
	CameraObject->Destroy();
}

void UCypherCameraComponent::PlaceCamera(FVector CamPos)
{
	AbilityInput = false;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UE_LOG(LogTemp, Warning, TEXT("PlaceCamera"));
	CameraObject = GetWorld()->SpawnActor<ACameraObject>(Camera, CamPos,FRotator::ZeroRotator, SpawnParams);

	if (CameraObject)
	{
		CameraObject->OwnerPawn = Cast<APawn>(MyOwner);	
	}
	
	CameraAction = ECameraAction::Default;
}

void UCypherCameraComponent::PossesCamera()
{
	if (!MyOwner->HasAuthority())
	{
		ServerPossesCamera();
	}
	
	AController* Controller = MyOwner->GetInstigatorController();
	
	if (Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("PossesCameraPlayer"));
		Controller->Possess(CameraObject);
	}	
}

void UCypherCameraComponent::RaycastCameraSelected()
{
	FVector StartLocation;
	FRotator Rotation;

	MyOwner = GetOwner();
	
	MyOwner->GetActorEyesViewPoint(StartLocation, Rotation);

	FVector TraceEnd = StartLocation + (Rotation.Vector() * Range);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;
	
	FHitResult Hit;

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, TraceEnd, ECC_WorldDynamic, QueryParams);
	
	if (Hit.bBlockingHit)
	{
		FVector ImpactPoint = Hit.ImpactPoint;
		TraceEnd = ImpactPoint;

		if (!CameraObject && AbilityInput)
		{
			CameraPos = ImpactPoint;
			PlaceCamera(CameraPos);
		}
		else if (Hit.GetActor() == CameraObject && AbilityInput)
		{
			RemoveCamera();
			return;
		}
	}

	if (AbilitySelected && CameraObject)
	{
		PossesCamera();			
	}
	
	if (AbilitySelected)
	{
		DrawDebugLine(GetWorld(), StartLocation, TraceEnd, FColor::White, false, 0.f,0.f,1.f);	
	}
}


void UCypherCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RaycastCameraSelected();
	/*	
	switch (CameraAction)
	{
		case ECameraAction::CameraSelected:
			RaycastCameraSelected();
			break;
		case ECameraAction::PlaceCamera:
			PlaceCamera(CameraPos);
			break;
		case ECameraAction::PossesCamera:
			PossesCameraPlayer();
			break;
		case ECameraAction::RemoveCamera:
			RemoveCamera();
			break;
		case ECameraAction::Default:
			break;
	}*/
}

void UCypherCameraComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilityComponent, AbilitySelected);
	DOREPLIFETIME(UAbilityComponent, Uses);	
}
