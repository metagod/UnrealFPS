// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "ExplosiveComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values for this component's properties
UExplosiveComponent::UExplosiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	hasExploded = false;
	// ...

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));

}


// Called when the game starts
void UExplosiveComponent::BeginPlay()
{
	Super::BeginPlay();

	RadialForce->Radius = ExplosionRadius;
	RadialForce->ForceStrength = ExplosionForce;
	RadialForce->ImpulseStrength = ExplosionImpulse;

	RadialForce->DestructibleDamage = 5.0;

	// ...
	
}


// Called every frame
void UExplosiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UExplosiveComponent::Explode()
{
	if (hasExploded)
		return;

	hasExploded = true;

	RadialForce->FireImpulse();

	UE_LOG (LogTemp, Warning, TEXT ("Boooooom !!"))
}

void UExplosiveComponent::StartTimerToExplosion()
{
	if (hasExploded || ExplosionTimerHandle.IsValid())
		return;

	GetOwner()->GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &UExplosiveComponent::Explode, ExplosionTimer, false);
}

void UExplosiveComponent::Reset()
{
	hasExploded = false;
	ExplosionTimerHandle.Invalidate();
}

