// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponActor.generated.h"

UCLASS()
class FPS_API ABaseWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponActor();

private :

	EWeaponState CurrentState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AFPSCharacter* MyOwner;
	class UWorld* World;
	class UPoolObjectComponent* ammoPool;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init(AFPSCharacter* owner);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual void OnFire();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual bool CanFire();

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/*Wepaon Data to modify the behaviour*/
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf <class AWeaponData> PrimaryWeaponDataRef;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf <class AWeaponData> SecondaryWeaponDataRef;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;
};


UENUM()
enum class EWeaponState : uint8
{
	READY,
	FIRING,
	RELOADING,
	EMPTY,
};