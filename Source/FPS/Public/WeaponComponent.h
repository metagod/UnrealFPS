// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
private :

	int AmmoRemaining;
	int FiredCount;
	void FireOnTime();
public:	

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
	void OnFire();
	
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void Reload();

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	FVector FP_MuzzleLocation;

	/*Wepaon Data to modify the behaviour*/
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf <class AWeaponData> PrimaryWeaponDataRef;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf <class AWeaponData> SecondaryWeaponDataRef;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	FORCEINLINE int GetCurrentAmmo() const { return AmmoRemaining; }
	int GetClipSize();

	virtual bool CanFire();
	virtual bool CanChangeMode(eWeaponMode newMode);
	virtual void FirePrimaryPressed();
	virtual void FireSecondaryPressed();
	virtual void OnPrimaryRelease();
	virtual void OnSecondaryRelease();

protected:
	FTimerHandle BurstTimeHandle;
	FTimerHandle RoFTimeHandle;
	FTimerHandle ReloadTimer;

	EWeaponStates CurrState;
	eWeaponMode	CurrMode;

	class AFPSCharacter* MyCharacter;
	class UWorld* World;
	class UPoolObjectComponent* ammoPool;

	void SpawnProjs();
	void SpawnFromPool(TSubclassOf<class AFPSProjectile> projectileType);
	void SpawnHitScan();
	void ResetFireTimer();
	void ReloadComplete();
	
	UFUNCTION(BluePrintNativeEvent, Category = Weapon)
	void SetPrimaryProperties();	//Set as soon as weapon is equipped
	UFUNCTION(BluePrintNativeEvent, Category = Weapon)
	void SetSecondaryProperties();	//Set as soon as secondary feature is weaopon is activated

	virtual void ChangeWeaponMode(eWeaponMode weaponMode);
	virtual void ChangeWeaponState(EWeaponStates weaponState);

	void SetPrimaryProperties_Implementation();
	void SetSecondaryProperties_Implementation();

private :
	AWeaponData* ActiveWeapon;
	bool bIsPrimaryKeyDown;
	bool bIsSecondaryKeyDown;

};

UENUM()
enum class EWeaponStates : uint8
{
	READY,
	FIRING,
	RELOADING,
	EMPTY,

	None = 231
};