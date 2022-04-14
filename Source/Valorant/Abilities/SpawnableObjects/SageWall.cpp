// Fill out your copyright notice in the Description page of Project Settings.


#include "SageWall.h"

#include "Components/BoxComponent.h"

// Sets default values
ASageWall::ASageWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(StaticMeshComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ASageWall::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->MaxHealth = SpawnMaxHealth;
	
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASageWall::OnHealthChanged);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_TimeWallSpawn, this, &ASageWall::SolidifyWall, TimeToSolidifyWall, false);
}

void ASageWall::OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	
}

void ASageWall::SolidifyWall()
{
	float percent = HealthComponent->CurrentHealth / HealthComponent->MaxHealth;

	HealthComponent->MaxHealth = SolidMaxHealth;

	HealthComponent->CurrentHealth = HealthComponent->MaxHealth * percent;
}

// Called every frame
void ASageWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

