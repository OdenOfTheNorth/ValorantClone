// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VisionSensingComponent.generated.h"

class AActor;
class UVisionSensingTargetComponent;
class UVisionSensingSettings;

USTRUCT(BlueprintType)
struct FFGVisionSensingResults
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AActor* SensedActor = nullptr;
	FVector SensedActorLocation;
	bool ActorInSight = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFGVisionSensingDelegate, const FFGVisionSensingResults&, Results);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UVisionSensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVisionSensingComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Direction, float DistanceMinimum) const;

	bool IsObjectVisible(const FVector& Origin, const FVector& Object) const;

	UPROPERTY(Transient)
	TArray<UVisionSensingTargetComponent*> SensedTargets;

	float GetVisionInRadians() const;

	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnTargetSensed;

	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnTargetLost;

	UPROPERTY(EditAnywhere, Category = Sensing)
	UVisionSensingSettings* SensingSettings = nullptr;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDebugDrawVision = false;
	
	UPROPERTY(EditAnywhere)
	float EnemyDetectionRange = 100;
private:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETraceTypeQuery> TraceType;

		
};
