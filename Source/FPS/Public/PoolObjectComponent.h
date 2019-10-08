// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolObjectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UPoolObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoolObjectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
		TSubclassOf<class AFPSProjectile> PooledObjectSubClass;

	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
		int poolSize;

	TArray<AFPSProjectile*> Pool;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetPool();

	void SetPoolType(TSubclassOf<class AFPSProjectile> prefab);

	AFPSProjectile* GetPoolObject();

	void ClearPool();
};
