#include "PlayerCharacter.h"
#include "Valorant/Weapon/BasicWeapon.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Valorant/HealthComponent.h"
#include "Valorant/Valorant.h"
#include "Net/UnrealNetwork.h"
#include "Valorant/Abilities/AbilityComponent.h"
#include "Valorant/Weapon/KnifeSlot.h"
#include "Valorant/Weapon/PrimaryWeapon.h"
#include "Valorant/Weapon/SecondaryWeapon.h"

APlayerCharacter::APlayerCharacter()
{ 	
	PrimaryActorTick.bCanEverTick = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;
	
	HealthComponent->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);

	if (IsLocallyControlled())
	{
		//USkeletalMeshComponent* MeshComp = GetMesh();
		//MeshComp->ToggleVisibility(false);
	}
	
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//Set up Primary weapon
		PrimaryWeapon = GetWorld()->SpawnActor<APrimaryWeapon>(PrimaryWeaponClass, FVector::ZeroVector,
													 FRotator::ZeroRotator, SpawnParams);
		PrimaryWeapon->SetOwner(this);
		PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
		PrimaryWeapon->SkeletonMeshComp->ToggleVisibility(false);
		//Set up Secondary Weapon
		SecondaryWeapon = GetWorld()->SpawnActor<ASecondaryWeapon>(SecondaryWeaponClass, FVector::ZeroVector,
											 FRotator::ZeroRotator, SpawnParams);
		SecondaryWeapon->SetOwner(this);
		SecondaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
		SecondaryWeapon->SkeletonMeshComp->ToggleVisibility(false);
		//Set up Knife weapon
		Knife = GetWorld()->SpawnActor<AKnifeSlot>(KnifeClass, FVector::ZeroVector,
											 FRotator::ZeroRotator, SpawnParams);
		Knife->SetOwner(this);
		Knife->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
		Knife->SkeletonMeshComp->ToggleVisibility(false);
		//Add weapons to a list
		CurrentWeapon.Add(PrimaryWeapon);
		CurrentWeapon.Add(SecondaryWeapon);
		CurrentWeapon.Add(Knife);
		//Set visibility on the selected weapon
		if (CurrentWeapon[CurrentWeaponIndex])
		{
			CurrentWeapon[CurrentWeaponIndex]->SkeletonMeshComp->ToggleVisibility(true);	
		}	
		/* Old Junk Code
		for (int i = 0; i < WeaponClasses.Num(); i++)
		{
			if (WeaponClasses[i] == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("WeaponClasses %d is null"), i)
				continue;
			}
		
			ABasicWeapon* WeaponsPtr = GetWorld()->SpawnActor<ABasicWeapon>(WeaponClasses[i], FVector::ZeroVector,
																 FRotator::ZeroRotator, SpawnParams);
			CurrentWeapon.Add(WeaponsPtr);
			CurrentWeapon[i]->SetOwner(this);
			CurrentWeapon[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
			CurrentWeapon[i]->SkeletonMeshComp->ToggleVisibility(false);
		}

		if (CurrentWeapon.Num() != 0)
		{
			CurrentWeapon[CurrentWeaponIndex]->SkeletonMeshComp->ToggleVisibility(true);	
		}	*/	
	}	
}

void APlayerCharacter::BeginCrouch()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginCrouch"));	
	Crouch();
}

void APlayerCharacter::EndCrouch()
{
	UE_LOG(LogTemp, Warning, TEXT("EndCrouch"));
	UnCrouch();
}

void APlayerCharacter::MoveForward(float value)
{
	if (!CanMove)
	{
		return;
	}

	AddMovementInput(GetActorForwardVector() * value);
}

void APlayerCharacter::MoveRight(float value)
{
	if (!CanMove)
	{
		return;
	}

	AddMovementInput(GetActorRightVector() * value);
}

void APlayerCharacter::LookUp(float value)
{
	if (!CanLook)
	{
		return;
	}

	AddControllerPitchInput(value);
}

void APlayerCharacter::LookRight(float value)
{
	if (!CanLook)
	{
		return;
	}

	AddControllerYawInput(value);
}

void APlayerCharacter::JumpInput()
{
	if (!CanMove)
	{
		return;
	}
	
	Jump();
}

void APlayerCharacter::StartShooting()
{
	if (Current_AbilityComp && Current_AbilityComp->AbilitySelected)
	{
		Current_AbilityComp->AbilityInput = true;
	}
	
	
	if (CurrentWeapon[CurrentWeaponIndex])
	{
		CurrentWeapon[CurrentWeaponIndex]->StartFire();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon is null"));
	}
}

void APlayerCharacter::EndShooting()
{
	if (Current_AbilityComp && Current_AbilityComp->AbilitySelected)
	{
		Current_AbilityComp->AbilityInput = false;
	}
	
	if (CurrentWeapon[CurrentWeaponIndex])
	{
		CurrentWeapon[CurrentWeaponIndex]->EndFire();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon is null"));
	}	
}

void APlayerCharacter::ReloadWeapon()
{
	if (CurrentWeapon[CurrentWeaponIndex])
	{
		CurrentWeapon[CurrentWeaponIndex]->StartReload();
	}
}

void APlayerCharacter::ChangeWeaponUp()
{
	if (!HasAuthority())
	{
		ServerChangeWeaponUp();
	}

	CurrentWeapon[CurrentWeaponIndex]->SkeletonMeshComp->ToggleVisibility(false);	
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % CurrentWeapon.Num();	
	CurrentWeapon[CurrentWeaponIndex]->SkeletonMeshComp->ToggleVisibility(true);
}

void APlayerCharacter::ChangeWeaponDown()
{
	if (!HasAuthority())
	{
		ServerChangeWeaponDown();
	}

	CurrentWeapon[CurrentWeaponIndex]->SkeletonMeshComp->ToggleVisibility(false);	
	CurrentWeaponIndex = (CurrentWeaponIndex - 1) % CurrentWeapon.Num();

	if (CurrentWeaponIndex == -1)
	{
		CurrentWeaponIndex = CurrentWeapon.Num() - 1;
	}
	
	CurrentWeapon[CurrentWeaponIndex]->SkeletonMeshComp->ToggleVisibility(true);
}

void APlayerCharacter::DropWeapon()
{
	if (!HasAuthority())
	{
		ServerDropWeapon();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;		
	
	//ABasicWeapon* WeaponsPtr = GetWorld()->SpawnActor<ABasicWeapon>(CurrentWeapon[CurrentWeaponIndex], FVector::ZeroVector,
	//												 FRotator::ZeroRotator, SpawnParams);

	ABasicWeapon* WeaponsPtr = CurrentWeapon[CurrentWeaponIndex];
	WeaponsPtr->SetOwner(nullptr);
	UPrimitiveComponent* primitive = Cast<UPrimitiveComponent>(WeaponsPtr);

	if (primitive)
	{
		FVector Forward = CameraComp->GetForwardVector();
		primitive->SetSimulatePhysics(true);
		primitive->AddImpulse(Forward * 250);
	}	

	CurrentWeapon[CurrentWeaponIndex] = nullptr;
}

void APlayerCharacter::PickUpChangeWeapon()
{
	if (!HasAuthority())
	{
		ServerPickUpChangeWeapon();
	}
}

void APlayerCharacter::ServerChangeWeaponUp_Implementation()
{
	ChangeWeaponUp();
}

bool APlayerCharacter::ServerChangeWeaponUp_Validate()
{
	return true;
}

void APlayerCharacter::ServerChangeWeaponDown_Implementation()
{
	ChangeWeaponDown();
}

bool APlayerCharacter::ServerChangeWeaponDown_Validate()
{
	return true;
}

void APlayerCharacter::Q_AbilityInput()
{
	if (!Q_AbilityComp)
	{
		UE_LOG(LogTemp,Warning, TEXT("no Q_AbilityComp"));
		return;
	}

	Q_AbilityBlueprint();
}

void APlayerCharacter::E_AbilityInput()
{
	if (!E_AbilityComp)
	{
		UE_LOG(LogTemp,Warning, TEXT("no E_AbilityComp"));
		return;
	}

	UE_LOG(LogTemp,Warning, TEXT("E_Ability"));

	E_AbilityBlueprint();
}

void APlayerCharacter::C_AbilityInput()
{
	if (!C_AbilityComp)
	{
		UE_LOG(LogTemp,Warning, TEXT("no C_AbilityComp APlayerCharacter"));
		return;
	}

	C_AbilityBlueprint();
}

void APlayerCharacter::X_AbilityInput()
{
	if (!X_AbilityComp)
	{
		UE_LOG(LogTemp,Warning, TEXT("no X_AbilityInput"));
		return;
	}

	X_AbilityBlueprint();
}

void APlayerCharacter::ServerDropWeapon_Implementation()
{
	DropWeapon();
}

bool APlayerCharacter::ServerDropWeapon_Validate()
{
	return true;
}

void APlayerCharacter::ServerPickUpChangeWeapon_Implementation()
{
	PickUpChangeWeapon();
}

bool APlayerCharacter::ServerPickUpChangeWeapon_Validate()
{
	return true;
}

void APlayerCharacter::StartSecondary()
{
	if (CurrentWeapon.Num() == 0)
	{
		return;
	}
	
	if (CurrentWeapon[CurrentWeaponIndex])
	{
		CurrentWeapon[CurrentWeaponIndex]->StartSecondary();
	}
}

void APlayerCharacter::EndSecondary()
{
	if (CurrentWeapon.Num() == 0)
	{
		return;
	}
	
	if (CurrentWeapon[CurrentWeaponIndex])
	{
		CurrentWeapon[CurrentWeaponIndex]->EndSecondary();
	}
}

void APlayerCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health,	float HealthDelta,
                                       const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0 && !Died)
	{
		Died = true;
		
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10);
	}	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	FHitResult hit;
	FVector StartLocation = GetPawnViewLocation();
	FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * 150);
	
	GetWorld()->LineTraceSingleByChannel(hit, StartLocation, EndLocation, ECC_WorldDynamic, QueryParams);

	if (hit.bBlockingHit)
	{
		ABasicWeapon* Weapon = Cast<ABasicWeapon>(hit.GetActor());

		if (Weapon)
		{
			DroppedWeapon = Weapon;
		}
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Move 
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	//Look
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::LookRight);
	//Change Weapon
	PlayerInputComponent->BindAction("MouseUp", IE_Pressed, this, &APlayerCharacter::ChangeWeaponUp);
	PlayerInputComponent->BindAction("MouseDown", IE_Pressed, this, &APlayerCharacter::ChangeWeaponDown);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartShooting);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::EndShooting);
	PlayerInputComponent->BindAction("Secondary", IE_Pressed, this, &APlayerCharacter::StartSecondary);
	PlayerInputComponent->BindAction("Secondary", IE_Released, this, &APlayerCharacter::EndSecondary);
	PlayerInputComponent->BindAction("Reload", IE_Released, this, &APlayerCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &APlayerCharacter::DropWeapon);	
	//Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::EndCrouch);
	//Abilitys
	PlayerInputComponent->BindAction("QAbility", IE_Pressed, this, &APlayerCharacter::Q_AbilityInput);
	PlayerInputComponent->BindAction("EAbility", IE_Pressed, this, &APlayerCharacter::E_AbilityInput);
	PlayerInputComponent->BindAction("CAbility", IE_Pressed, this, &APlayerCharacter::C_AbilityInput);
	PlayerInputComponent->BindAction("XAbility", IE_Pressed, this, &APlayerCharacter::X_AbilityInput);	
}

FVector APlayerCharacter::GetPawnViewLocation() const
{
	if (CameraComp != nullptr)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, CurrentWeapon);
	DOREPLIFETIME(APlayerCharacter, Died);
}

