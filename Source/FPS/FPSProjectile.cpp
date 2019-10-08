// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FPS.h"
#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);		// set up a notification for when this component hits something blocking
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	//ProjectileMovement->InitialSpeed = 3000.f;
	//ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

void AFPSProjectile::SetMaxSpeed(float _maxSpeed)
{
	ProjectileMovement->MaxSpeed = _maxSpeed;
}

void AFPSProjectile::SetInitialSpeed(float _initialSpeed)
{
	ProjectileMovement->InitialSpeed = _initialSpeed;
}

void AFPSProjectile::ChangeLifeSpan(float _timeInSeconds)
{
	InitialLifeSpan = _timeInSeconds;
	SetLifeSpan(_timeInSeconds);
}

void AFPSProjectile::ToggleSniperMode(bool _toggleState)
{
	if (_toggleState)
	{
		ProjectileMovement->InitialSpeed = 10000;
		ProjectileMovement->MaxSpeed = 10000;
		ProjectileMovement->ProjectileGravityScale = 0;
	}
	else
	{
		ProjectileMovement->InitialSpeed = 3000;
		ProjectileMovement->MaxSpeed = 3000;
		ProjectileMovement->ProjectileGravityScale = 1;
	}
}

void AFPSProjectile::SetActive(bool toggle)
{
	isActive = toggle;
	SetActorHiddenInGame(!toggle);
}

bool AFPSProjectile::IsActorActive()
{
	return isActive;
}