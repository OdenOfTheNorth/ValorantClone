#include "VisionSensingComponent.h"
#include "VisionSensingSettings.h"
#include "VisionSensingTargetComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

UVisionSensingComponent::UVisionSensingComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UVisionSensingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SensingSettings == nullptr)
		return;

	const FVector Direction = GetOwner()->GetActorForwardVector();
	const FVector Origin = GetOwner()->GetActorLocation();

	if (bDebugDrawVision)
	{
		FVector Right = Direction.RotateAngleAxis(SensingSettings->Angle, FVector::UpVector);
		FVector Left = Direction.RotateAngleAxis(-SensingSettings->Angle, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * SensingSettings->DistanceMinimum, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * SensingSettings->DistanceMinimum, FLinearColor::Green);
	}

	for (int32 Index = SensedTargets.Num() - 1; Index >= 0; --Index)
	{
		UVisionSensingTargetComponent* Target = SensedTargets[Index];

		if (Target == nullptr || (Target != nullptr && Target->IsBeingDestroyed()))
		{
			SensedTargets.RemoveAt(Index);
			continue;
		}
		
		if (!IsPointVisible(Target->GetTargetOrigin(), Origin, Direction, SensingSettings->DistanceMinimum))
		{
			FFGVisionSensingResults Results;
			Results.SensedActor = Target->GetOwner();
			Results.SensedActorLocation = Target->GetOwner()->GetActorLocation();
			Results.ActorInSight = false;
			OnTargetLost.Broadcast(Results);
			SensedTargets.RemoveAt(Index);
		}
	}

	TArray<UVisionSensingTargetComponent*> ListOfTargets;
	
	UVisionSensingTargetComponent::GetSensingTargets(ListOfTargets, GetOwner()->GetActorLocation(), SensingSettings->DistanceMinimum);

	for (UVisionSensingTargetComponent* Target : ListOfTargets)
	{		
		if (!SensedTargets.Contains(Target) &&
			IsPointVisible(Target->GetTargetOrigin(), Origin, Direction, SensingSettings->DistanceMinimum))
		{
			SensedTargets.Add(Target);
			FFGVisionSensingResults Results;
			Results.SensedActor = Target->GetOwner();
			Results.SensedActorLocation = Target->GetOwner()->GetActorLocation();
			Results.ActorInSight = true;
			OnTargetSensed.Broadcast(Results);
		}
	}
}

bool UVisionSensingComponent::IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Direction, float DistanceMinimum) const
{
	if (SensingSettings == nullptr)
		return false;

	float DistanceMinimumSq = FMath::Square(DistanceMinimum);

	if (FVector::DistSquared(Origin, PointToTest) > DistanceMinimumSq)
	{
		return false;
	}

	const FVector DirectionToTarget = (PointToTest - Origin).GetSafeNormal();

	const float AsHalfRad = FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	const bool bIsValid = Dot > AsHalfRad;

	if (bIsValid)
	{
		return IsObjectVisible(Origin, PointToTest);
	}

	return false;
}

bool UVisionSensingComponent::IsObjectVisible(const FVector& Origin, const FVector& Object) const
{
	FHitResult Hit;
	FVector Direction = (Object - Origin);
	Direction.Normalize();
	Direction *= EnemyDetectionRange * 1000;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	
	if (GetWorld()->LineTraceSingleByChannel(Hit, Origin, Direction, ECC_GameTraceChannel1, QueryParams))
	{
		//Cheak if an actor was hit. 
		
		auto* actor = Hit.GetActor();		
		if (!actor)	return false;
		
		FString output = Hit.Actor->GetName();

		//Cheak if actor location is the same as the player location.
		//if they are same then you have hit the player;
		
		if (actor->GetActorLocation() == Object)		
		{
			UE_LOG(LogTemp, Warning,TEXT("%s"), *output);
			DrawDebugLine(GetWorld(),Origin,Hit.GetActor()->GetActorLocation(),FColor::Orange,false,0.5,0,6 );
			return true;	
		}
		else
		{
			return false;
		}					
		/*UFGVisionSensingTargetComponent* Target = Hit.Actor->FindComponentByClass<UFGVisionSensingTargetComponent>();
		if (Target)
		{
			//UE_LOG(LogTemp, Warning,TEXT("%s"), *output);
			DrawDebugLine(GetWorld(),Origin,Hit.GetActor()->GetActorLocation(),FColor::Orange,false,0.5,0,6 );
			return true;	
		}*/
	}

	return false;
}

float UVisionSensingComponent::GetVisionInRadians() const
{
	if (SensingSettings == nullptr)
		return 0.0;

	return FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
}

