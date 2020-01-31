// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExplosiveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UExplosiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExplosiveComponent();

private:

	class URadialForceComponent* RadialForce;

	bool hasExploded;

	FTimerHandle ExplosionTimerHandle;
protected:

	UPROPERTY(EditAnywhere, Category = "Explosion Properties")
	float ExplosionForce;

	UPROPERTY(EditAnywhere, Category = "Explosion Properties")
	float ExplosionImpulse;

	UPROPERTY(EditAnywhere, Category = "Explosion Properties")
	int ExplosionRadius;

	UPROPERTY(EditAnywhere, Category = "Explosion Properties")
	float ExplosionTimer;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION (BlueprintCallable, Category = "Explosion")
	void Explode();
	
	UFUNCTION(BlueprintCallable, Category = "Explosion")
	void StartTimerToExplosion();

	UFUNCTION(BlueprintCallable, Category = "Explosion")
	void Reset();
};
