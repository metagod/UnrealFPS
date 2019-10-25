// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

UCLASS(config=Game)
class AFPSProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	bool isActive = false;
protected:
	FTimerHandle TimeToLive;

public:
	AFPSProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Projectile)
	float LifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	float ImpactForce;


	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void ToggleSniperMode(bool _toggleState);
	void SetMaxSpeed(float _maxSpeed);
	void SetInitialSpeed(float _initialSpeed);
	void ChangeLifeSpan(float _timeInSeconds);
	void SetActive(bool toggle);
	bool IsActorActive();
	void SetInactiveTimer(float time);
	void OnTimerOver();
	void OnFire();
};

