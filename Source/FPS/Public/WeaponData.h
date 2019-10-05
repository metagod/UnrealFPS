// Fill out your copyright notice in the Description page of Project Settings.

///Summmary
/*
This Class will serve as container to store data for creating different weapons
*/
///
#pragma once

#include "GameFramework/Actor.h"
#include "WeaponData.generated.h"

UCLASS()
class FPS_API AWeaponData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponData();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	///Primary Options
	
	//Is Hit scan : if true weapon uses raycast instead of actual projectiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bIsHitScan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float HitScanRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float ClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float ReloadTime;


	//PROJECTILE Life Span
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float timeInSeconds;

	/// Projs Fired Per Second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int BurstAmount;

	//If 0, all the burst amount bullets are fired simultaneously
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float BurstRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float Spread;

	//Change from default fov, this is not the absolute value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float FOVChange;

	//Recoil
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float PitchShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float RollShake;

	//SECONDARY WEAPON PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Secondary)
	bool bActivateOnHold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float FOVChangeSecondary;


};
