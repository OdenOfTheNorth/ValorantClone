#include "Valorant/Abilities/Characters/Cypher/TripWireComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

void UTripWireComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTripWireComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RaycastCameraSelected();
}

void UTripWireComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UTripWireComponent::ServerPlaceTripWire_Implementation(FVector TripPos, FVector Rot)
{
	PlaceTripWire(TripPos, Rot);
}

bool UTripWireComponent::ServerPlaceTripWire_Validate(FVector TripPos, FVector Rot)
{
	return true;
}

void UTripWireComponent::PlaceTripWire(FVector TripPos, FVector Rot)
{
	if (!MyOwner->HasAuthority())
	{
		ServerPlaceTripWire(TripPos, Rot);
	}
	
	AbilityInput = false;
	AbilitySelected = false;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UE_LOG(LogTemp, Warning, TEXT("PlaceTripWire"));
	
	ATripWire* Wire = GetWorld()->SpawnActor<ATripWire>(TripWire, TripPos, Rot.Rotation(), SpawnParams);

	if (Wire)
	{
		Wire->MyOwner = MyOwner;		
	}
}

void UTripWireComponent::RemoveTripWire(ATripWire* Wire)
{
	Wire->Destroy();
}

void UTripWireComponent::RaycastCameraSelected()
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

		ATripWire* HitTripWire = Cast<ATripWire>(Hit.GetActor());

		FHitResult HitTrip;
			
		GetWorld()->LineTraceSingleByChannel(HitTrip, ImpactPoint, ImpactPoint + (Hit.Normal * 600), ECC_WorldDynamic, QueryParams);
			
		
		if (AbilitySelected)
		{
			//TripWire
			DrawDebugLine(GetWorld(), StartLocation, TraceEnd, FColor::Green, false, 0.f,0.f,1.f);

			if (HitTrip.bBlockingHit)
			{
				DrawDebugLine(GetWorld(), ImpactPoint, ImpactPoint + (Hit.Normal * 600), FColor::Green, false, 0.f,0.f,1.f);	
			} else
			{
				DrawDebugLine(GetWorld(), ImpactPoint, ImpactPoint + (Hit.Normal * 600), FColor::Red, false, 0.f,0.f,1.f);
			}			
		}
		
		if (HitTripWire && AbilityInput)
		{
			RemoveTripWire(HitTripWire);
		}
		else if (AbilityInput)
		{
			if (HitTrip.bBlockingHit)
			{
				PlaceTripWire(ImpactPoint, Hit.Normal);
			}			
		}
	}
	
	//if (AbilitySelected)
	//{
	//	DrawDebugLine(GetWorld(), StartLocation, TraceEnd, FColor::White, false, 0.f,0.f,1.f);	
	//}
}

void UTripWireComponent::Start()
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
}

void UTripWireComponent::End()
{
	Super::End();
}

void UTripWireComponent::StartDeploy()
{
	Super::StartDeploy();
}

void UTripWireComponent::EndDeploy()
{
	Super::EndDeploy();
}

void UTripWireComponent::ServerStart_Implementation()
{
}

bool UTripWireComponent::ServerStart_Validate()
{
	return true;
}

void UTripWireComponent::ServerEnd_Implementation()
{
}

bool UTripWireComponent::ServerEnd_Validate()
{
	return true;
}