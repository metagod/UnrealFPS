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

	// ...
}

void UWeaponComponent::Init(AFPSCharacter* _ref)
{
	if (NULL != _ref)
	{
		MyCharacter = _ref;

		ChangeWeaponState(eWeaponStates::READY);
		
		ammoPool = GetOwner()->FindComponentByClass<UPoolObjectComponent>();
		
		if (ammoPool != nullptr)
		{
			ammoPool->CreateComplexPool(PrimaryWeaponDataRef.GetDefaultObject()->ProjectileClass);
			if (SecondaryWeaponDataRef != NULL)
				ammoPool->CreateComplexPool(SecondaryWeaponDataRef.GetDefaultObject()->ProjectileClass);
		}


		FP_MuzzleLocation = MyCharacter->GetMuzzleComponent();
		ChangeWeaponMode(eWeaponMode::PRIMARY);
	}
}

void UWeaponComponent::ResetFireTimer()
{
	ChangeWeaponState(eWeaponStates::READY);
}

void UWeaponComponent::OnFire()
{
	if (!CanFire())
		return;

	ChangeWeaponState(eWeaponStates::FIRING);

	MyCharacter->GetWorldTimerManager().SetTimer(RoFTimeHandle, this, &UWeaponComponent::ResetFireTimer, ActiveWeapon->RateOfFire, false);

	FiredCount = ActiveWeapon->BurstAmount;
	FireOnTime();
}

void UWeaponComponent::OnSecondaryFire()
{

}


void UWeaponComponent::FireOnTime()
{
	--AmmoConsumed;
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

	if (AmmoConsumed == 0)
	{
		ChangeWeaponState(eWeaponStates::RELOADING);
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
	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : MyCharacter->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	AFPSProjectile* _spawndProj = ammoPool->GetPoolObjectOfType(projectileType);
	if (_spawndProj != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Spawned"));

		_spawndProj->SetMaxSpeed(ActiveWeapon->ProjectileSpeed);
		_spawndProj->SetInitialSpeed(ActiveWeapon->ProjectileSpeed);
		_spawndProj->SetActorLocation(SpawnLocation);
		_spawndProj->SetActorRotation(SpawnRotation);
		_spawndProj->SetInactiveTimer(3);
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

	if (World == NULL)
		World = GetWorld();

	if (World != NULL && PrimaryWeaponDataRef != NULL)
	{
		float _randomFloatX = FMath::FRandRange(-PrimaryWeaponDataRef.GetDefaultObject()->Spread, PrimaryWeaponDataRef.GetDefaultObject()->Spread);
		float _randomFloatY = FMath::FRandRange(-PrimaryWeaponDataRef.GetDefaultObject()->Spread, PrimaryWeaponDataRef.GetDefaultObject()->Spread);

		const FRotator SpawnRotation = MyCharacter->GetControlRotation().Add(_randomFloatX, _randomFloatY, 0);
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : MyCharacter->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		PrimaryWeaponDataRef.GetDefaultObject()->ProjectileClass.GetDefaultObject()->SetMaxSpeed(PrimaryWeaponDataRef.GetDefaultObject()->ProjectileSpeed);
		PrimaryWeaponDataRef.GetDefaultObject()->ProjectileClass.GetDefaultObject()->SetInitialSpeed(PrimaryWeaponDataRef.GetDefaultObject()->ProjectileSpeed);
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ActorSpawnParams.Template = PrimaryWeaponDataRef.GetDefaultObject()->ProjectileClass.GetDefaultObject();

		AFPSProjectile* _spawndProj = World->SpawnActor<AFPSProjectile>(ActorSpawnParams.Template->GetClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
		if (_spawndProj->LifeSpan > 0)
			_spawndProj->SetLifeSpan(_spawndProj->LifeSpan);
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
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : MyCharacter->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
	
		const FVector SpawnRange = SpawnLocation + SpawnDirection * ActiveWeapon->HitScanRange;
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, MyCharacter);
		TraceParams.bReturnPhysicalMaterial = true;
		//TraceParams.bTraceAsyncScene = true;
		FHitResult _out (ForceInit);
		
		World->LineTraceSingleByChannel(_out, SpawnLocation, SpawnRange, ECollisionChannel::ECC_WorldDynamic, TraceParams);

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
	AmmoConsumed = PrimaryWeaponDataRef.GetDefaultObject()->ClipSize;
	ChangeWeaponState(eWeaponStates::READY);
}

bool UWeaponComponent::CanFire()
{
	return CurrState == READY;
}

void UWeaponComponent::FirePrimary()
{
	ChangeWeaponMode(eWeaponMode::PRIMARY);
	OnFire();
	
}

void UWeaponComponent::FireSecondary()
{
	ChangeWeaponMode(eWeaponMode::SECONDARY);
	if (!ActiveWeapon->bActivateOnHold)
		OnFire();
}

bool UWeaponComponent::CanChangeMode(eWeaponMode newMode)
{
	if (newMode == CurrMode)
		return false;				//No need to change mode

	if (newMode == eWeaponMode::PRIMARY && CurrMode == eWeaponMode::SECONDARY)
	{
		return !SecondaryWeaponDataRef.GetDefaultObject()->bActivateOnHold;
	}
	else if (newMode == eWeaponMode::SECONDARY && CurrMode == eWeaponMode::PRIMARY)
	{
		return true;
	}

	return true;
}

void UWeaponComponent::SetSecondaryProperties_Implementation()
{
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

void UWeaponComponent::ChangeWeaponState(eWeaponStates weaponState)
{
	if (CurrState == weaponState)
		return;

	CurrState = weaponState;

	switch (weaponState)
	{
	case eWeaponStates::READY: break;
	case eWeaponStates::RELOADING: Reload(); break;
	case eWeaponStates::FIRING: break;
	default: break;
	}
}
