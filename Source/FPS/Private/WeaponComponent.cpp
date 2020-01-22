// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "WeaponData.h"
#include "PoolObjectComponent.h"
#include "../Public/WeaponComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsPrimaryKeyDown = false;
	bIsSecondaryKeyDown = false;
	FiredCount = 0;
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UWeaponComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (bIsPrimaryKeyDown || (bIsSecondaryKeyDown && !ActiveWeapon->bActivateOnHold))
	{
		OnFire();
	}

	// ...
}

void UWeaponComponent::Init(AFPSCharacter* _ref)
{
	if (NULL != _ref)
	{
		MyCharacter = _ref;

		ChangeWeaponState(EWeaponStates::READY);
		
		if (ammoPool == nullptr)
		{
			ammoPool = GetOwner()->FindComponentByClass<UPoolObjectComponent>();

			if (PrimaryWeaponDataRef != NULL)
				ammoPool->CreateComplexPool(PrimaryWeaponDataRef.GetDefaultObject()->ProjectileClass, PrimaryWeaponDataRef.GetDefaultObject()->ClipSize);
			if (SecondaryWeaponDataRef != NULL)
				ammoPool->CreateComplexPool(SecondaryWeaponDataRef.GetDefaultObject()->ProjectileClass, SecondaryWeaponDataRef.GetDefaultObject()->ClipSize);
		}
		
		ChangeWeaponMode(eWeaponMode::PRIMARY);
		AmmoRemaining = ActiveWeapon->ClipSize;
	}
}

void UWeaponComponent::ResetFireTimer()
{
	///Can't ovverride reloading state

	if (CurrState != EWeaponStates::RELOADING)
		ChangeWeaponState(EWeaponStates::READY);
}

void UWeaponComponent::OnFire()
{
	if (!CanFire())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Firing"));

	FP_MuzzleLocation = GetOwner()->GetActorLocation() + FVector(2, 0, 0);

	ChangeWeaponState(EWeaponStates::FIRING);

	MyCharacter->GetWorldTimerManager().SetTimer(RoFTimeHandle, this, &UWeaponComponent::ResetFireTimer, ActiveWeapon->RateOfFire, false);

	FiredCount = ActiveWeapon->BurstAmount;
	FireOnTime();
}


void UWeaponComponent::FireOnTime()
{	
	// try and fire a projectile
	if (!ActiveWeapon->bIsHitScan)
	{
		// spawn the projectile at the muzzle TODO : Implement Pooling
		SpawnProjs();
	}
	else
	{
		SpawnHitScan();
	}

	--AmmoRemaining;

	if (AmmoRemaining <= 0)
	{	
		ChangeWeaponState(EWeaponStates::RELOADING);
		return;
	}

	if (FiredCount > 1)
	{
		--FiredCount;
		
		if (PrimaryWeaponDataRef.GetDefaultObject()->BurstRate > 0)
			MyCharacter->GetWorldTimerManager().SetTimer(BurstTimeHandle, this, &UWeaponComponent::FireOnTime, ActiveWeapon->BurstRate, false);
		else
			FireOnTime();
		
	}
}

void UWeaponComponent::SpawnFromPool(TSubclassOf<AFPSProjectile> projectileType)
{
	float _randomFloatX = FMath::FRandRange(-PrimaryWeaponDataRef.GetDefaultObject()->Spread, ActiveWeapon->Spread);
	float _randomFloatY = FMath::FRandRange(-PrimaryWeaponDataRef.GetDefaultObject()->Spread, ActiveWeapon->Spread);

	const FRotator SpawnRotation = MyCharacter->GetControlRotation().Add(_randomFloatX, _randomFloatY, 0);
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	
	AFPSProjectile* _spawndProj = ammoPool->GetPoolObjectOfType(projectileType);
	if (_spawndProj != nullptr)
	{	
		_spawndProj->GetCollisionComp()->IgnoreActorWhenMoving(MyCharacter, true);
		MyCharacter->MoveIgnoreActorAdd(_spawndProj);

		_spawndProj->SetMaxSpeed(ActiveWeapon->ProjectileSpeed);
		_spawndProj->SetInitialSpeed(ActiveWeapon->ProjectileSpeed);
		_spawndProj->SetActorLocation(FP_MuzzleLocation);
		_spawndProj->SetActorRotation(SpawnRotation);
		_spawndProj->SetInactiveTimer(ActiveWeapon->projectileLifeSpan);
		_spawndProj->SetActive(true);
		_spawndProj->OnFire();
	}
}

void UWeaponComponent::SpawnProjs()
{
	if (ammoPool != nullptr)
	{
		SpawnFromPool(ActiveWeapon->ProjectileClass);
		return;
	}
}

void UWeaponComponent::SpawnHitScan()
{
	if (World == NULL)
		World = GetWorld();

	if (World != NULL && ActiveWeapon != nullptr)
	{
		float _randomFloat = FMath::FRandRange(0, ActiveWeapon->Spread);
		const FRotator SpawnRotation = MyCharacter->GetControlRotation().Add(0, _randomFloat, _randomFloat);
		const FVector SpawnDirection = SpawnRotation.Vector();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		
		const FVector SpawnRange = FP_MuzzleLocation + SpawnDirection * ActiveWeapon->HitScanRange;
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, MyCharacter);
		TraceParams.bReturnPhysicalMaterial = true;
		//TraceParams.bTraceAsyncScene = true;
		FHitResult _out (ForceInit);
		
		World->LineTraceSingleByChannel(_out, FP_MuzzleLocation, SpawnRange, ECollisionChannel::ECC_WorldDynamic, TraceParams);

		if (NULL != _out.GetActor())
		{
			UPrimitiveComponent* OtherComp = Cast<UPrimitiveComponent> (_out.GetActor()->GetRootComponent());
			if (NULL != OtherComp)
				OtherComp->AddImpulseAtLocation(SpawnDirection * 400000, _out.ImpactPoint);
		}
	}
}

void UWeaponComponent::Reload()
{
	MyCharacter->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UWeaponComponent::ReloadComplete, PrimaryWeaponDataRef.GetDefaultObject()->ReloadTime, false);
}

void UWeaponComponent::ReloadComplete()
{
	AmmoRemaining = PrimaryWeaponDataRef.GetDefaultObject()->ClipSize;
	ChangeWeaponState(EWeaponStates::READY);
}

inline int UWeaponComponent::GetClipSize()
{
	return ActiveWeapon->ClipSize;
}

bool UWeaponComponent::CanFire()
{
	return CurrState == EWeaponStates::READY;
}

void UWeaponComponent::SetSecondaryProperties_Implementation()
{
	if (SecondaryWeaponDataRef == nullptr)
		return;

	MyCharacter->GetFirstPersonCameraComponent()->FieldOfView += SecondaryWeaponDataRef.GetDefaultObject()->FOVChange;
	MyCharacter->Recoil = SecondaryWeaponDataRef.GetDefaultObject()->Recoil;
	ActiveWeapon = SecondaryWeaponDataRef.GetDefaultObject();
}

void UWeaponComponent::SetPrimaryProperties_Implementation()
{
	MyCharacter->GetFirstPersonCameraComponent()->FieldOfView += PrimaryWeaponDataRef.GetDefaultObject()->FOVChange;
	MyCharacter->Recoil = PrimaryWeaponDataRef.GetDefaultObject()->Recoil;
	ActiveWeapon = PrimaryWeaponDataRef.GetDefaultObject();
}

#pragma region Listeners
void UWeaponComponent::FirePrimaryPressed()
{
	OnFire();
}

void UWeaponComponent::FireSecondaryPressed()
{
	ChangeWeaponMode(eWeaponMode::SECONDARY);
	OnFire();
}

void UWeaponComponent::OnPrimaryRelease()
{
	bIsPrimaryKeyDown = false;
}

void UWeaponComponent::OnSecondaryRelease()
{
	ChangeWeaponMode(eWeaponMode::PRIMARY);
	bIsSecondaryKeyDown = false;
}

#pragma endregion Listeners

bool UWeaponComponent::CanChangeMode(eWeaponMode newMode)
{
	if (newMode == CurrMode)
		return false;				//No need to change mode

	if (newMode == eWeaponMode::PRIMARY && CurrMode == eWeaponMode::SECONDARY)
	{
		return !SecondaryWeaponDataRef.GetDefaultObject()->bActivateOnHold && bIsSecondaryKeyDown;
	}
	else if (newMode == eWeaponMode::SECONDARY && CurrMode == eWeaponMode::PRIMARY)
	{
		return true;
	}

	return true;
}


void UWeaponComponent::ChangeWeaponMode(eWeaponMode weaponMode)
{	
	switch (weaponMode)
	{
	case eWeaponMode::PRIMARY: SetPrimaryProperties(); break;
	case eWeaponMode::SECONDARY: SetSecondaryProperties(); break;
		default:
			break;
	}	
}

void UWeaponComponent::ChangeWeaponState(EWeaponStates weaponState)
{	
	if (CurrState == weaponState)
		return;

	CurrState = weaponState;

	switch (weaponState)
	{
	case EWeaponStates::READY: break;
	case EWeaponStates::RELOADING: Reload(); break;
	case EWeaponStates::FIRING: break;
	default: break;
	}
}
