#include "Valorant/Abilities/Characters/Sova/SovaArrowAbilityComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Valorant/Abilities/SpawnableObjects/Arrow.h"
#include "Math/UnrealMathUtility.h"

USovaArrowAbilityComponent::USovaArrowAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USovaArrowAbilityComponent::BeginPlay()
{
	Super::BeginPlay();	
	MyOwner = GetOwner();
}

void USovaArrowAbilityComponent::Start()
{
	Super::Start();

	FireArrow();
	return;
	
	if (true)
	{
		
	}

	if (!AbilitySelected)
	{
		AbilitySelected = true;
	}
	else
	{
		StartDeploy();
	}
}

void USovaArrowAbilityComponent::End()
{
	Super::End();
}

void USovaArrowAbilityComponent::StartDeploy()
{
	Super::StartDeploy();

	AbilityInput = true;
}

void USovaArrowAbilityComponent::EndDeploy()
{
	Super::EndDeploy();
}

void USovaArrowAbilityComponent::ServerFireArrow_Implementation()
{
	FireArrow();
}

bool USovaArrowAbilityComponent::ServerFireArrow_Validate()
{
	return true;
}

void USovaArrowAbilityComponent::FireArrow()
{
	if (!MyOwner->HasAuthority())
	{
		ServerFireArrow();
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector StartLocation;
	FRotator Rotation;	
			
	MyOwner->GetActorEyesViewPoint(StartLocation, Rotation);

	FVector spawnPos = StartLocation + Rotation.Vector() * 150;
	
	AArrow* ArrowActor = GetWorld()->SpawnActor<AArrow>(Arrow, spawnPos, Rotation, SpawnParams);

	if (!ArrowActor)
	{
		return;
	}

	Force = 1500;
	
	FVector Impulse = Rotation.Vector() * Force;

	ArrowActor->ProjectileMovementComponent->Velocity = Impulse;
}

void USovaArrowAbilityComponent::ServerStart_Implementation()
{
	
}

bool USovaArrowAbilityComponent::ServerStart_Validate()
{
	return true;
}




// Called every frame
void USovaArrowAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AbilitySelected && AbilityInput)
	{
		LastFrameAbilityInput = AbilityInput;
		//CanShoot = true;
		Force += ForceBuildUp * DeltaTime;
		Force = FMath::Clamp(Force, 0.f, MaxForce);
	}

	if (AbilitySelected && AbilityInput != LastFrameAbilityInput)
	{

		FireArrow();

		/*
		UPrimitiveComponent* primitive = Cast<UPrimitiveComponent>(ArrowActor);
		
		if (primitive && primitive->IsSimulatingPhysics())
		{			
			primitive->AddImpulse(Impulse);
		}*/

		//CanShoot = false;
	}


	LastFrameAbilityInput = AbilityInput;
	// ...
}

void USovaArrowAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilityComponent, AbilitySelected);
	DOREPLIFETIME(UAbilityComponent, Uses);	
}

