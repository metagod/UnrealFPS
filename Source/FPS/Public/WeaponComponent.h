// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
private :

	int AmmoConsumed;
	int FiredCount;
	void FireOnTime();
public:	
	
	enum eWeaponStates
	{
		READY, 
		FIRING, 
		RELOADING, 
		EMPTY, 

		None = 999
	};

	enum eWeaponMode
	{
		PRIMARY,
		SECONDARY,

		NONE = 99
	};
	
	// Sets default values for this component's properties
	UWeaponComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void Init(class AFPSCharacter* _ref);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void OnPrimaryFire();
	
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void OnSecondaryFire();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void Reload();

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

	virtual bool CanFire();
	virtual void ChangeWeaponMode(eWeaponMode weaponMode);

protected:
	FTimerHandle BurstTimeHandle;
	FTimerHandle RoFTimeHandle;
	FTimerHandle ReloadTimer;

	eWeaponStates CurrState;
	eWeaponMode	CurrMode;

	class AFPSCharacter* MyCharacter;
	class UWorld* World;
	class UPoolObjectComponent* ammoPool;

	void SpawnProjs();
	void SpawnFromPool(TSubclassOf<class AFPSProjectile> projectileType);
	void SpawnHitScan();
	void ResetFireTimer();
	void ReloadComplete();
	bool bIsFiring;

	UFUNCTION(BluePrintNativeEvent, Category = Weapon)
	void SetPrimaryProperties();	//Set as soon as weapon is equipped
	UFUNCTION(BluePrintNativeEvent, Category = Weapon)
	void SetSecondaryProperties();	//Set as soon as secondary feature is weaopon is activated

	void SetPrimaryProperties_Implementation();
	void SetSecondaryProperties_Implementation();
};
