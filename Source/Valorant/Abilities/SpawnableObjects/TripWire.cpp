// Fill out your copyright notice in the Description page of Project Settings.


#include "Valorant/Abilities/SpawnableObjects/TripWire.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
//#include "Valorant/Valorant.h"
#include "Valorant/Abilities/Possessing/CameraObject.h"
#include "Valorant/Player/PlayerCharacter.h"

ATripWire::ATripWire()
{
 	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	
	StartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartMesh"));
	StartMesh->SetupAttachment(SceneComponent);

	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndMesh"));
	EndMesh->SetupAttachment(StartMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATripWire::OnHealthChanged);
}

void ATripWire::OnServerTrippwireActivated_Implementation()
{
	OnTrippwireActivated();
}

bool ATripWire::OnServerTrippwireActivated_Validate()
{
	return true;
}

void ATripWire::OnServerTrippwireDeactivated_Implementation()
{
	OnTrippwireDeactivated();
}

bool ATripWire::OnServerTrippwireDeactivated_Validate()
{
	return true;
}

void ATripWire::OnTrippwireActivated()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dose Not Have Authority"));
		OnServerTrippwireActivated();		
	}

	Activated = false;
	
	APlayerCharacter* Pawn = Cast<APlayerCharacter>(TrapedActor);
	if (Pawn)
	{
		ActorTrappedLocation = Pawn->GetActorLocation();
		//Pawn->CanMove = false;
		//Pawn->GetInstigatorController()->SetIgnoreMoveInput(false);		
		//Pawn->GetMovementComponent()->Deactivate();
		GetWorld()->GetTimerManager().SetTimer(TimeHandler_TripWire, this,&ATripWire::OnTrippwireDeactivated, TrappedTime, false);
	}
}

void ATripWire::OnTrippwireDeactivated()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dose Not Have Authority"));
		OnServerTrippwireDeactivated();		
	}
	
	APlayerCharacter* Pawn = Cast<APlayerCharacter>(TrapedActor);

	if (Pawn)
	{
		Pawn->CanMove = true;

		if (MyOwner)
		{
			UGameplayStatics::ApplyDamage(Pawn, Damage, MyOwner->GetInstigatorController(), MyOwner, TripWireDamageType);		
		}		
	}

	Destroy();
}

void ATripWire::ServerActorTrappedInTripwire_Implementation()
{
	ActorTrappedInTripwire();
}

bool ATripWire::ServerActorTrappedInTripwire_Validate()
{
	return true;
}

void ATripWire::ActorTrappedInTripwire()
{
	if (!MyOwner->HasAuthority())
	{
		ServerActorTrappedInTripwire();
	}
	
	FVector dir = TrapedActor->GetActorLocation() - ActorTrappedLocation;
	float dist = FVector::Distance(ActorTrappedLocation, TrapedActor->GetActorLocation());
	APlayerCharacter* character = Cast<APlayerCharacter>(TrapedActor);

	if (character)
	{
		FHitResult Hit;
		FVector DeltaToMove = dir * dist;
		character->AddActorWorldOffset(DeltaToMove, true, &Hit);
	}
}

void ATripWire::OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta,
                                const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0)
	{
		if (TrapedActor)
		{
			APlayerCharacter* Pawn = Cast<APlayerCharacter>(TrapedActor);
			if (Pawn)
			{
				Pawn->CanMove = true;
			}

			ACameraObject* CameraObject = Cast<ACameraObject>(TrapedActor);
			if (CameraObject)
			{
				CameraObject->CanMove = true;
			}			
		}

		Destroy();
	}
}

void ATripWire::BeginPlay()
{
	Super::BeginPlay();

	//MyOwner = GetOwner();
	StartLocation = StartMesh->GetComponentLocation();
	ShotDirection = GetActorForwardVector();	
	TraceEnd = StartLocation + (ShotDirection * Range);
	
	FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	//QueryParams.bReturnPhysicalMaterial = true;
	
	FHitResult Hit;

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, TraceEnd, ECC_WorldDynamic, QueryParams);
	
	if (Hit.bBlockingHit)
	{
		EndMesh->SetWorldLocation(Hit.ImpactPoint);
		//UE_LOG(LogTemp, Warning, TEXT("pos %s,%s,%s"),Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z);
		UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *Hit.Actor->GetName());
		
		DrawDebugLine(GetWorld(), StartLocation, Hit.ImpactPoint, FColor::White, false, 0.f,0.f,1.f);

		DrawDebugSphere(GetWorld(), StartLocation, 50, 16, FColor::Red, false, 100);
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 50, 16, FColor::Blue, false, 100);
	}
	else
	{
		DrawDebugLine(GetWorld(), StartLocation, Hit.ImpactPoint, FColor::White, false, 0.f,0.f,1.f);
		UE_LOG(LogTemp, Warning, TEXT("Did not hit"));
	}
}

void ATripWire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Activated)
	{

		
		return;
	}

	StartLocation = StartMesh->GetComponentLocation();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);

	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FHitResult Hit;

	//UE_LOG(LogTemp, Warning, TEXT("ATripWire"));
		
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, TraceEnd, ECC_WorldDynamic, QueryParams))
	{		
		DrawDebugLine(GetWorld(), StartLocation, Hit.ImpactPoint, FColor::White, false, 0.f,0.f,1.f);
		
		TrapedActor = Hit.GetActor();
		APlayerCharacter* Player = Cast<APlayerCharacter>(TrapedActor);

		if (Player)
		{
			OnTrippwireActivated();
		}		
	}	
}

