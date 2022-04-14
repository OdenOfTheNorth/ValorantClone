#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Valorant/Weapon/BasicWeapon.h"
#include "PlayerCharacter.generated.h"


class APrimaryWeapon;
class ASecondaryWeapon;
class AKnifeSlot;
class UAbilityComponent;
class UCameraComponent;
class USpringArmComponent;
class ABasicWeapon;
class UHealthComponent;

UENUM()
enum ETeam
{
	Red,
	Blue,
	FFA
};

UCLASS()
class VALORANT_API APlayerCharacter : public ACharacter
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
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	//TArray<TSubclassOf<ABasicWeapon>> WeaponClasses;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FName WeaponAttachSocketName = "WeaponSocket";
	
	UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TArray<ABasicWeapon*> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<APrimaryWeapon> PrimaryWeaponClass;
	UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	APrimaryWeapon* PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<ASecondaryWeapon> SecondaryWeaponClass;
	UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	ASecondaryWeapon* SecondaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<AKnifeSlot> KnifeClass;
	UPROPERTY(Replicated ,EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	AKnifeSlot* Knife;

	//UPROPERTY(Replicated ,VisibleAnywhere, BlueprintReadWrite, Category = "Weapons")
	//ABasicWeapon* CurrentWeapon;
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Components")
	UAbilityComponent* Q_AbilityComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Components")
	UAbilityComponent* E_AbilityComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Components")
	UAbilityComponent* C_AbilityComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	UAbilityComponent* X_AbilityComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Abilitys")
	UAbilityComponent* Current_AbilityComp;
	
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

	
	float RunSpeed, WalkSpeed, SlowWalkSpeed, CrouchSpeed;

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

	ABasicWeapon* DroppedWeapon;
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual FVector GetPawnViewLocation() const override;


};
