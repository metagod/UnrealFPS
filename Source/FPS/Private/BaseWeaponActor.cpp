// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "BaseWeaponActor.h"
#include "WeaponData.h"
#include "PoolObjectComponent.h"

// Sets default values
ABaseWeaponActor::ABaseWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeaponActor::Init(AFPSCharacter * owner)
{
	MyOwner = owner;

	ammoPool = FindComponentByClass<UPoolObjectComponent>();
	ammoPool->CreateComplexPool(ProjectileClass, PrimaryWeaponDataRef.GetDefaultObject()->ClipSize);	
}

void ABaseWeaponActor::OnFire()
{
}

void ABaseWeaponActor::Reload()
{
}

bool ABaseWeaponActor::CanFire()
{
	return false;
}

