
#pragma once
#include "CoreMinimal.h"
#include "Valorant/Abilities/AbilityComponent.h"
#include "TeleportComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UTeleportComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeleportComponent();

	virtual void Start() override;
	//virtual void End() override;
	//virtual void StartDeploy() override;
	//virtual void EndDeploy() override;
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerStart();
	//UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	//void ServerEnd();
	//UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	//virtual void ServerStartDeploy();
	//UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	//virtual void ServerEndDeploy();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerStartTeleport();
	void StartTeleport();
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerSetPosition();
	void SetPosition();

	FVector TeleportPos;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(Replicated ,EditDefaultsOnly)
	float Range = 1000;
	UPROPERTY(Replicated ,EditDefaultsOnly)
	float IsTeleporting = false;
	
	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TeleportTime = 1.f;
	
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* TeleportEffect;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FTimerHandle TimerHandle_TeleportTime;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
