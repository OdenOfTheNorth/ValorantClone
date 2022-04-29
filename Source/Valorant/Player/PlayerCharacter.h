#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Valorant/Weapon/BasicWeapon.h"
#include "AbilitySystemInterface.h"
#include "Valorant/Abilities/ValorantAttributeSet.h"
#include "Valorant/Abilities/ValorantAbilitySystemComponent.h"
#include "Valorant/Abilities/ValorantGameplayAbility.h"
#include "PlayerCharacter.generated.h"

class UAbility;
class UPrimaryWeapon;
class USecondaryWeapon;
class UKnifeSlot;
class UAbilityComponent;
class UCameraComponent;
class USpringArmComponent;
class UBasicWeapon;
class UHealthComponent;

UENUM()
enum ETeam
{
	Red,
	Blue,
	FFA
};

UCLASS()
class VALORANT_API APlayerCharacter : public ACharacter ,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETeam> Team;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilitys", meta =(AllowPrivateAccess = "true"))
	class UValorantAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilitys", meta =(AllowPrivateAccess = "true"))
	const UValorantAttributeSet* AttributeSetBase;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilitys")
	TArray<TSubclassOf<UValorantGameplayAbility>> DefaultAbilities;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	//TArray<TSubclassOf<ABasicWeapon>> WeaponClasses;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FName WeaponAttachSocketName = "WeaponSocket";
	
	UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TArray<UBasicWeapon*> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<UPrimaryWeapon> PrimaryWeaponClass;
	UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	UPrimaryWeapon* PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<USecondaryWeapon> SecondaryWeaponClass;
	UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	USecondaryWeapon* SecondaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<UKnifeSlot> KnifeClass;
	UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	UKnifeSlot* Knife;

	//UPROPERTY(Replicated ,VisibleAnywhere, BlueprintReadWrite, Category = "Weapons")
	//ABasicWeapon* CurrentWeapon;

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	int CurrentWeaponIndex = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	int Money = 0;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool Died = false;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool CanMove = true;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool CanLook = true;

	/*	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	//UAbilityComponent* Q_AbilityComp;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	//UAbilityComponent* E_AbilityComp;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	//UAbilityComponent* C_AbilityComp;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	//UAbilityComponent* X_AbilityComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	TSubclassOf<UAbility> Q_AbilityClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	TSubclassOf<UAbility> E_AbilityClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	TSubclassOf<UAbility> C_AbilityClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	TSubclassOf<UAbility> X_AbilityClass;
	
	UPROPERTY( BlueprintReadWrite, Replicated, Category = "Abilitys")
	UAbility* Q_Ability;
	UPROPERTY( BlueprintReadWrite, Replicated, Category = "Abilitys")
	UAbility* E_Ability;
	UPROPERTY( BlueprintReadWrite, Replicated, Category = "Abilitys")
	UAbility* C_Ability;
	UPROPERTY( BlueprintReadWrite, Replicated, Category = "Abilitys")
	UAbility* X_Ability;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	UHands* Current_HandsComp;
	
	void Q_AbilityInput();
	void E_AbilityInput();
	void C_AbilityInput();
	void X_AbilityInput();

	UFUNCTION(BlueprintImplementableEvent)
	void Q_AbilityBlueprint();
	UFUNCTION(BlueprintImplementableEvent)
	void E_AbilityBlueprint();
	UFUNCTION(BlueprintImplementableEvent)
	void C_AbilityBlueprint();
	UFUNCTION(BlueprintImplementableEvent)
	void X_AbilityBlueprint();
	*/
	
	float	RunSpeed,
			WalkSpeed,
			SlowWalkSpeed,
			CrouchSpeed;

	UPROPERTY(BlueprintReadOnly)
	float DefaultFOV;
	UPROPERTY(BlueprintReadWrite)
	bool ActivatSecondary = false;
	
protected:
	virtual void BeginPlay() override;

	void BeginCrouch();
	void EndCrouch();

	void MoveForward(float value);
	void MoveRight(float value);
	
	void LookUp(float value);
	void LookRight(float value);
	
	void JumpInput();

	void StartShooting();
	void EndShooting();

	void ReloadWeapon();

	void ChangeWeaponUp();
	void ChangeWeaponDown();
	void DropWeapon();
	void PickUpChangeWeapon();      
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeWeaponUp();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeWeaponDown();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropWeapon();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickUpChangeWeapon();

	UFUNCTION(BlueprintCallable)
	void StartSecondary();
	UFUNCTION(BlueprintCallable)
	void EndSecondary();

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health,	float HealthDelta,
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UBasicWeapon* DroppedWeapon;


	
public:	
	virtual void Tick(float DeltaTime) override;	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	
	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY()
	UHands* ObjectArray;
};
