// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "PoolObjectComponent.h"
#include "FPSProjectile.h"

// Sets default values for this component's properties
UPoolObjectComponent::UPoolObjectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPoolObjectComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPoolObjectComponent::SetPoolType(TSubclassOf<class AFPSProjectile> prefab)
{
	PooledObjectSubClass = prefab;
}

void UPoolObjectComponent::SetPool()
{
	UWorld* world = GetWorld();
	if (world)
	{
		for (int i = 0; i < poolSize; ++i)
		{
			AFPSProjectile* poolEntry = world->SpawnActor<AFPSProjectile>(PooledObjectSubClass, FVector().ZeroVector, FRotator().ZeroRotator);
			poolEntry->ChangeLifeSpan(0.f);
			poolEntry->SetActive(false);
			Pool.Add(poolEntry);
		}
	}
}


// Called every frame
void UPoolObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AFPSProjectile* UPoolObjectComponent::GetPoolObject()
{
	for (AFPSProjectile* projectile : Pool)
	{
		if (!projectile->IsActorActive())
			return projectile;
	}

	return nullptr;
}