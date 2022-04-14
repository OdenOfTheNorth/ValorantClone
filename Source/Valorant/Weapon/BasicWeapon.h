#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Valorant/Player/PlayerCharacter.h"
#include "Hands.h"
#include "BasicWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class APlayerCharacter;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	UPROPERTY()
	FVector_NetQuantize TraceEnd;	
};

UCLASS()
class VALORANT_API ABasicWeapon : public AHands
{
	GENERATED_BODY()
	
public:	
	ABasicWeapon();

	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	UFUNCTION(BlueprintCallable)
	void StartReload();
	UFUNCTION(BlueprintCallable)
	void EndReload();

	UFUNCTION(BlueprintCallable)
	void StartSecondary();
	UFUNCTION(BlueprintCallable)
	void EndSecondary();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EndFire();

	void PlayFireEffects(FVector TraceEnd);
	void PlaySurfaceEffect(FVector ImpactPoint, EPhysicalSurface SurfaceType);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	bool Automatic = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float Damage = 20;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float HeadShotMultiplier = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Components")
	USkeletalMeshComponent* SkeletonMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Components")
	UStaticMeshComponent* StaticMeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* MuzzleLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanADS;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool WantToADS;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ADS_FOV = 70;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ADS_Speed = 70;
	
	APlayerCharacter* Player;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FallOfRange = 6000;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FallOfMultiplier = 0.7;
	
	/* Bullets Per Minute */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ReloadTime;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* FleshImpactEffect;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	USoundBase* FireSound;	

	FTimerHandle TimerHandle_ReloadTime;
	FTimerHandle TimerHandle_TimeBetweenShots;

	float TimeBetweenShots = 0.5f;
	float LastFireTime = 0;



	bool CanShot = true;
	
	UPROPERTY(EditDefaultsOnly , Category = "Weapon")
	int MaxAmmo = 25;

	UPROPERTY(VisibleAnywhere , Category = "Weapon")
	int CurrentAmmo;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:	
	virtual void Tick(float DeltaTime) override;
};
