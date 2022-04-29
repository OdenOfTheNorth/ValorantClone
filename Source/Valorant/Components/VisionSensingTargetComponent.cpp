#include "VisionSensingTargetComponent.h"
#include "GameFramework/Actor.h"

TArray<UVisionSensingTargetComponent*> UVisionSensingTargetComponent::TargetsList;

void UVisionSensingTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetsList.Add(this);
}

void UVisionSensingTargetComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	TargetsList.Remove(this);

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

FVector UVisionSensingTargetComponent::GetTargetOrigin() const
{
	return GetOwner()->GetActorLocation();
}

void UVisionSensingTargetComponent::GetSensingTargets(TArray<UVisionSensingTargetComponent*>& OutTargets, const FVector& Origin, float DistanceMin)
{
	const float DistanceMinSq = FMath::Square(DistanceMin);

	for (UVisionSensingTargetComponent* Target : TargetsList)
	{
		if (Target == nullptr)
			continue;

		float DistanceSq = FVector::DistSquared(Target->GetOwner()->GetActorLocation(), Origin);
		if (DistanceSq < DistanceMinSq)
		{
			OutTargets.Add(Target);
		}
	}
}

