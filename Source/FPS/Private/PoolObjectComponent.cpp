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

void UPoolObjectComponent::CreateComplexPool(TSubclassOf<class AFPSProjectile> prefab, int size)
{
	check(prefab != NULL)

	if (!complexPool.Contains(prefab))
	{
		poolSize = size == 0 ? poolSize : size;

		TArray<AFPSProjectile*> complexArray; 

		UWorld* world = GetWorld();
		if (world)
		{
			for (int i = 0; i < poolSize; ++i)
			{
				AFPSProjectile* poolEntry = world->SpawnActor<AFPSProjectile>(prefab, FVector().ZeroVector, FRotator().ZeroRotator);
				poolEntry->ChangeLifeSpan(0.f);
				poolEntry->SetActive(false);
				complexArray.Add(poolEntry);
			}
		}

		complexPool.Add(prefab, complexArray);
	}
}

AFPSProjectile* UPoolObjectComponent::GetPoolObjectOfType(TSubclassOf<class AFPSProjectile> prefab)
{
	check(complexPool.Contains(prefab));

	for (AFPSProjectile* projectile : complexPool[prefab])
	{
		if (!projectile->IsActorActive())
			return projectile;
	}

	return nullptr;
}

// Called every frame
void UPoolObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPoolObjectComponent::ClearPool()
{
	if (complexPool.Num() == 0)
		return;

	for (auto& pool : complexPool)
	{
		for (int i = 0; i < poolSize; ++i)
		{
			pool.Value[i]->Destroy();
			pool.Value[i] = nullptr;
		}
	}

	complexPool.Empty();
}