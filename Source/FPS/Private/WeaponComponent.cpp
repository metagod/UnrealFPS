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
	CurrState = READY;
	ammoPool = GetOwner()->FindComponentByClass<UPoolObjectComponent>();
	if (ammoPool != nullptr)
	{
		ammoPool->ClearPool();
		ammoPool->SetPoolType(PrimaryProjectileClass);
		ammoPool->SetPool();
	}// ...
	
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
		FP_MuzzleLocation = MyCharacter->GetMuzzleComponent();
		SetPrimaryProperties();
	}
}

void UWeaponComponent::ResetFireTimer()
{
	CurrState = READY;
}

void UWeaponComponent::OnPrimaryFire()
{
	if (!CanFire())
		return;

	if (WeaponDataRef.GetDefaultObject()->BurstRate > 0)
		bIsFiring = true;
	
	CurrState = FIRING;

	MyCharacter->GetWorldTimerManager().SetTimer(RoFTimeHandle, this, &UWeaponComponent::ResetFireTimer, WeaponDataRef.GetDefaultObject()->RateOfFire, false);

	FiredCount = WeaponDataRef.GetDefaultObject()->BurstAmount;
	FireOnTime();
}

void UWeaponComponent::OnSecondaryFire()
{

}


void UWeaponComponent::FireOnTime()
{
	// try and fire a projectile
	if (!WeaponDataRef.GetDefaultObject()->bIsHitScan && PrimaryProjectileClass != NULL)
	{
		// spawn the projectile at the muzzle TODO : Implement Pooling
		SpawnProjs();
	}
	else
	{
		SpawnHitScan();
	}

	if (FiredCount > 1)
	{
		--FiredCount;
		--AmmoConsumed;

		if (WeaponDataRef.GetDefaultObject()->BurstRate > 0)
			MyCharacter->GetWorldTimerManager().SetTimer(BurstTimeHandle, this, &UWeaponComponent::FireOnTime, WeaponDataRef.GetDefaultObject()->BurstRate, false);
		else
			FireOnTime();
		
	}
	else
	{
		bIsFiring = false;
		return;
	}

}

void UWeaponComponent::SpawnFromPool()
{
	float _randomFloatX = FMath::FRandRange(-WeaponDataRef.GetDefaultObject()->Spread, WeaponDataRef.GetDefaultObject()->Spread);
	float _randomFloatY = FMath::FRandRange(-WeaponDataRef.GetDefaultObject()->Spread, WeaponDataRef.GetDefaultObject()->Spread);

	const FRotator SpawnRotation = MyCharacter->GetControlRotation().Add(_randomFloatX, _randomFloatY, 0);
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : MyCharacter->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	AFPSProjectile* _spawndProj = ammoPool->GetPoolObject();
	if (_spawndProj != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Spawned"));

		_spawndProj->SetMaxSpeed(WeaponDataRef.GetDefaultObject()->ProjectileSpeed);
		_spawndProj->SetInitialSpeed(WeaponDataRef.GetDefaultObject()->ProjectileSpeed);
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
		SpawnFromPool();
		return;
	}

	if (World == NULL)
		World = GetWorld();

	if (World != NULL && WeaponDataRef != NULL)
	{
		float _randomFloatX = FMath::FRandRange(-WeaponDataRef.GetDefaultObject()->Spread, WeaponDataRef.GetDefaultObject()->Spread);
		float _randomFloatY = FMath::FRandRange(-WeaponDataRef.GetDefaultObject()->Spread, WeaponDataRef.GetDefaultObject()->Spread);

		const FRotator SpawnRotation = MyCharacter->GetControlRotation().Add(_randomFloatX, _randomFloatY, 0);
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : MyCharacter->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		PrimaryProjectileClass.GetDefaultObject()->SetMaxSpeed(WeaponDataRef.GetDefaultObject()->ProjectileSpeed);
		PrimaryProjectileClass.GetDefaultObject()->SetInitialSpeed(WeaponDataRef.GetDefaultObject()->ProjectileSpeed);
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ActorSpawnParams.Template = PrimaryProjectileClass.GetDefaultObject();

		AFPSProjectile* _spawndProj = World->SpawnActor<AFPSProjectile>(ActorSpawnParams.Template->GetClass(), SpawnLocation, SpawnRotation, ActorSpawnParams);
		if (_spawndProj->LifeSpan > 0)
			_spawndProj->SetLifeSpan(_spawndProj->LifeSpan);
	}
}

void UWeaponComponent::SpawnHitScan()
{
	if (World == NULL)
		World = GetWorld();

	if (World != NULL && WeaponDataRef != NULL)
	{
		float _randomFloat = FMath::FRandRange(0, WeaponDataRef.GetDefaultObject()->Spread);
		const FRotator SpawnRotation = MyCharacter->GetControlRotation().Add(0, _randomFloat, _randomFloat);
		const FVector SpawnDirection = SpawnRotation.Vector();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : MyCharacter->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
	
		const FVector SpawnRange = SpawnLocation + SpawnDirection * WeaponDataRef.GetDefaultObject()->HitScanRange;
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
	CurrState = RELOADING;
	MyCharacter->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UWeaponComponent::ReloadComplete, WeaponDataRef.GetDefaultObject()->ReloadTime, false);
}

void UWeaponComponent::ReloadComplete()
{
	CurrState = READY;
	AmmoConsumed = WeaponDataRef.GetDefaultObject()->ClipSize;
}

bool UWeaponComponent::CanFire()
{
	return !bIsFiring && CurrState == READY;
}

void UWeaponComponent::SetSecondaryProperties_Implementation()
{}

void UWeaponComponent::SetPrimaryProperties_Implementation()
{}


void UWeaponComponent::ChangeWeaponMode(eWeaponMode weaponMode)
{
	if (CurrMode != weaponMode)
	{
		switch (weaponMode)
		{
		case eWeaponMode::PRIMARY: SetPrimaryProperties(); break;
		case eWeaponMode::SECONDARY: SetSecondaryProperties(); break;
			default:
				break;
		}
	}
}
