#include "PlayerCharacter.h"
#include "Valorant/Weapon/BasicWeapon.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Valorant/HealthComponent.h"
#include "Valorant/Valorant.h"
#include "Net/UnrealNetwork.h"		
#include "Valorant/Abilities/Ability.h"
//#include "Valorant/Abilities/AbilityComponent.h"
#include "Valorant/Weapon/KnifeSlot.h"
#include "Valorant/Weapon/PrimaryWeapon.h"
#include "Valorant/Weapon/SecondaryWeapon.h"
#include "Engine/ActorChannel.h"
#include "Valorant/Valorant.h"
#include "Valorant/Abilities/ValorantAbilitySystemComponent.h"
#include "Valorant/Abilities/ValorantAttributeSet.h"
#include "Valorant/Abilities/ValorantGameplayAbility.h"
#include <GameplayEffectTypes.h>

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
{ 	
	PrimaryActorTick.bCanEverTick = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);	

	AbilitySystemComponent = CreateDefaultSubobject<UValorantAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//AttributeSetBase = CreateDefaultSubobject<UValorantAttributeSet>(TEXT("AttributeSetBase"));
	
	//AttributeSetBase = AbilitySystemComponent->GetSet<UValorantAttributeSet>();
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APlayerCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle AGEH = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void APlayerCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UValorantGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			if (StartupAbility == nullptr)
			{
				continue;
			}
			
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartupAbility,1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	InitializeAttributes();
	GiveAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	InitializeAttributes();

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInputID",
			static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	/*
	if (Q_AbilityClass)
	{
		Q_Ability = NewObject<UAbility>(this, Q_AbilityClass, FName("Q_Ability"));	
	}
	if (E_AbilityClass)
	{
		E_Ability = NewObject<UAbility>(this, E_AbilityClass, FName("E_Ability"));	
	}
	if (C_AbilityClass)
	{
		C_Ability = NewObject<UAbility>(this, C_AbilityClass, FName("C_Ability"));	
	}
	if (X_AbilityClass)
	{
		X_Ability = NewObject<UAbility>(this, X_AbilityClass, FName("X_Ability"));	
	}
	
	
	
	Q_Ability = GetWorld()->SpawnActor<UAbility>(Q_AbilityClass,
	 FVector::ZeroVector,FRotator::ZeroRotator, SpawnParams);
	E_Ability = GetWorld()->SpawnActor<UAbility>(E_AbilityClass,
	 FVector::ZeroVector,FRotator::ZeroRotator, SpawnParams);
	C_Ability = GetWorld()->SpawnActor<UAbility>(C_AbilityClass,
	 FVector::ZeroVector,FRotator::ZeroRotator, SpawnParams);
	X_Ability = GetWorld()->SpawnActor<UAbility>(X_AbilityClass,
	 FVector::ZeroVector,FRotator::ZeroRotator, SpawnParams);*/
	/*
	if (Q_Ability)
	{
		Q_Ability->MyOwner = Cast<AActor>(this);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Failed To spawn Q_Ability"));
	}
	
	if (E_Ability)
	{
		E_Ability->MyOwner = Cast<AActor>(this);
	}	
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Failed To spawn E_Ability"));
	}
	
	if (C_Ability)
	{
		C_Ability->MyOwner = Cast<AActor>(this);
	}	
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Failed To spawn C_Ability"));
	}
	
	if (X_Ability)
	{
		X_Ability->MyOwner = Cast<AActor>(this);
	}	
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Failed To spawn X_Ability"));
	}*/
	
	DefaultFOV = CameraComp->FieldOfView;	
	HealthComponent->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);

	if (IsLocallyControlled())
	{
		//USkeletalMeshComponent* MeshComp = GetMesh();
		//MeshComp->ToggleVisibility(false);
	}
	
	if (HasAuthority())
	{
		if (PrimaryWeaponClass)
		{
			//Set up Primary weapon
			PrimaryWeapon = NewObject<UPrimaryWeapon>(this, PrimaryWeaponClass, FName("PrimaryWeapon"));
			
			//PrimaryWeapon = GetWorld()->SpawnActor<UPrimaryWeapon>(PrimaryWeaponClass, FVector::ZeroVector,
			//											 FRotator::ZeroRotator, SpawnParams);
			//PrimaryWeapon->MyOwner = this;
			//PrimaryWeapon->SkeletonMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
			//PrimaryWeapon->SkeletonMeshComp->ToggleVisibility(false);
		}

		if (SecondaryWeaponClass)
		{
			//Set up Secondary Weapon
			SecondaryWeapon = NewObject<USecondaryWeapon>(this, SecondaryWeaponClass, FName("SecondaryWeapon"));
			//SecondaryWeapon = GetWorld()->SpawnActor<USecondaryWeapon>(SecondaryWeaponClass, FVector::ZeroVector,
			//									 FRotator::ZeroRotator, SpawnParams);
			//SecondaryWeapon->MyOwner = this;
			//SecondaryWeapon->SkeletonMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
			//SecondaryWeapon->SkeletonMeshComp->ToggleVisibility(false);
		}

		if (KnifeClass)
		{
			//Set up Knife weapon
			Knife = NewObject<UKnifeSlot>(this, KnifeClass, FName("Knife"));			
			//Knife = GetWorld()->SpawnActor<UKnifeSlot>(KnifeClass, FVector::ZeroVector,
			//									 FRotator::ZeroRotator, SpawnParams);
			//Knife->MyOwner = this;
			//Knife->SkeletonMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
			//Knife->SkeletonMeshComp->ToggleVisibility(false);
		}
		
		//Add weapons to a list
		CurrentWeapon.Add(PrimaryWeapon);
		CurrentWeapon.Add(SecondaryWeapon);
		CurrentWeapon.Add(Knife);
		//Set visibility on the selected weapon
		if (CurrentWeapon[CurrentWeaponIndex])
		{
			//CurrentWeapon[CurrentWeaponIndex]->SkeletonMeshComp->ToggleVisibility(true);	
		}

		//Current_HandsComp = CurrentWeapon[CurrentWeaponIndex];
		
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
{/*
	if (Current_HandsComp)
	{
		Current_HandsComp->StartUsing();
	}
	
	
	if (Current_HandsComp && Current_HandsComp->AbilitySelected)
	{
		Current_HandsComp->AbilityInput = true;
	}
		
	if (CurrentWeapon[CurrentWeaponIndex])
	{
		CurrentWeapon[CurrentWeaponIndex]->StartUsing();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon is null"));
	}*/
}

void APlayerCharacter::EndShooting()
{/*
	if (Current_HandsComp)
	{
		Current_HandsComp->StopUsing();	
	}
	
	/*
	if (Current_AbilityComp && Current_AbilityComp->AbilitySelected)
	{
		Current_AbilityComp->AbilityInput = false;
	}
	
	if (CurrentWeapon[CurrentWeaponIndex])
	{
		CurrentWeapon[CurrentWeaponIndex]->StopUsing();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon is null"));
	}*/
}

void APlayerCharacter::ReloadWeapon()
{/*
	UBasicWeapon* Weapon = Cast<UBasicWeapon>(Current_HandsComp);

	//Current_HandsComp->
	
	if (Weapon)
	{		
		Weapon->StartReload();
	}*/
}

void APlayerCharacter::ChangeWeaponUp()
{
	if (!HasAuthority())
	{
		ServerChangeWeaponUp();
	}

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % CurrentWeapon.Num();
	//Current_HandsComp = Cast<UHands>(CurrentWeapon[CurrentWeaponIndex]);
}

void APlayerCharacter::ChangeWeaponDown()
{
	if (!HasAuthority())
	{
		ServerChangeWeaponDown();
	}

	CurrentWeaponIndex = (CurrentWeaponIndex - 1) % CurrentWeapon.Num();

	if (CurrentWeaponIndex == -1)
	{
		CurrentWeaponIndex = CurrentWeapon.Num() - 1;
	}

	//Current_HandsComp = Cast<UHands>(CurrentWeapon[CurrentWeaponIndex]);
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

	UBasicWeapon* WeaponsPtr = CurrentWeapon[CurrentWeaponIndex];
	WeaponsPtr->MyOwner = nullptr;
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
/*
void APlayerCharacter::Q_AbilityInput()
{
	/*if (!Q_Ability)
	{
		UE_LOG(LogTemp,Warning, TEXT("no Q_Ability"));
	}
	else
	{
		Current_HandsComp = Q_Ability;
		Current_HandsComp->StartUsing();
		return;
	}	
	
	/*if (!Q_AbilityComp)
	{
		UE_LOG(LogTemp,Warning, TEXT("no Q_AbilityComp"));		
	}
	el

	Q_AbilityBlueprint();	
}

void APlayerCharacter::E_AbilityInput()
{/*
	if (!E_Ability)
	{
		UE_LOG(LogTemp,Warning, TEXT("no E_Ability"));
	}
	else
	{
		E_Ability->StartUsing();
		return;
	}	
	
	/*if (!E_AbilityComp)
	{
		UE_LOG(LogTemp,Warning, TEXT("no E_AbilityComp"));
		return;
	}

	UE_LOG(LogTemp,Warning, TEXT("E_Ability"));

	E_AbilityBlueprint();
}

void APlayerCharacter::C_AbilityInput()
{/*
	if (!C_Ability)
	{
		UE_LOG(LogTemp,Warning, TEXT("no E_Ability"));
	}
	else
	{
		C_Ability->StartUsing();
		return;
	}	
	
	/*if (!C_AbilityComp)
	{
		UE_LOG(LogTemp,Warning, TEXT("no C_AbilityComp APlayerCharacter"));
		return;
	}

	C_AbilityBlueprint();
}

void APlayerCharacter::X_AbilityInput()
{/*
	if (!X_Ability)
	{
		UE_LOG(LogTemp,Warning, TEXT("no E_Ability"));
	}
	else
	{
		X_Ability->StartUsing();
		return;
	}	
	
	/*if (!X_AbilityComp)
	{
		UE_LOG(LogTemp,Warning, TEXT("no X_AbilityInput"));
		return;
	}

	X_AbilityBlueprint();
}*/

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
		UBasicWeapon* Weapon = Cast<UBasicWeapon>(hit.GetActor());

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
	//PlayerInputComponent->BindAction("QAbility", IE_Pressed, this, &APlayerCharacter::Q_AbilityInput);
	//PlayerInputComponent->BindAction("EAbility", IE_Pressed, this, &APlayerCharacter::E_AbilityInput);
	//PlayerInputComponent->BindAction("CAbility", IE_Pressed, this, &APlayerCharacter::C_AbilityInput);
	//PlayerInputComponent->BindAction("XAbility", IE_Pressed, this, &APlayerCharacter::X_AbilityInput);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInputID",
			static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
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

bool APlayerCharacter::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	//bWroteSomething |= Channel->ReplicateSubobject(Current_HandsComp, *Bunch, *RepFlags);

	return bWroteSomething;
}

