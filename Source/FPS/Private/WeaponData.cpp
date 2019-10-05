// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "WeaponData.h"


// Sets default values
AWeaponData::AWeaponData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponData::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponData::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

