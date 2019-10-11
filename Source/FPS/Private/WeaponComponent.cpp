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

		CurrState = READY;
		
		ammoPool = GetOwner()->FindComponentByClass<UPoolObjectComponent>();
		
		if (ammoPool != nullptr)
		{
			ammoPool->CreateComplexPool(PrimaryWeaponDataRef.GetDefaultObject()->ProjectileClass);
			if (SecondaryWeaponDataRef != NULL)
				ammoPool->CreateComplexPool(SecondaryWeaponDataRef.GetDefaultObject()->ProjectileClass);
		}


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

	if (PrimaryWeaponDataRef.GetDefaultObject()->BurstRate > 0)
		bIsFiring = true;
	
	CurrState = FIRING;

	MyCharacter->GetWorldTimerManager().SetTimer(RoFTimeHandle, this, &UWeaponComponent::ResetFireTimer, PrimaryWeaponDataRef.GetDefaultObject()->RateOfFire, false);

	FiredCount = PrimaryWeaponDataRef.GetDefaultObject()->BurstAmount;
	FireOnTime();
}

void UWeaponComponent::OnSecondaryFire()
{

}


void UWeaponComponent::FireOnTime()
{
	// try and fire a projectile
	if (!PrimaryWeaponDataRef.GetDefaultObject()->bIsHitScan)
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

		if (PrimaryWeaponDataRef.GetDefaultObject()->BurstRate > 0)
			MyCharacter->GetWorldTimerManager().SetTimer(BurstTimeHandle, this, &UWeaponComponent::FireOnTime, PrimaryWeaponDataRef.GetDefaultObject()->BurstRate, false);
		else
			FireOnTime();
		
	}
	else
	{
		bIsFiring = false;
		return;
	}

}

void UWeaponComponent::SpawnFromPool(TSubclassOf<AFPSProjectile> projectileType)
{
	float _randomFloatX = FMath::FRandRange(-PrimaryWeaponDataRef.GetDefaultObject()->Spread, PrimaryWeaponDataRef.GetDefaultObject()->Spread);
	float _randomFloatY = FMath::FRandRange(-PrimaryWeaponDataRef.GetDefaultObject()->Spread, PrimaryWeaponDataRef.GetDefaultObject()->Spread);

	const FRotator SpawnRotation = MyCharacter->GetControlRotation().Add(_randomFloatX, _randomFloatY, 0);
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : MyCharacter->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	AFPSProjectile* _spawndProj = ammoPool->GetPoolObjectOfType(projectileType);
	if (_spawndProj != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Spawned"));

		_spawndProj->SetMaxSpeed(PrimaryWeaponDataRef.GetDefaultObject()->ProjectileSpeed);
		_spawndProj->SetInitialSpeed(PrimaryWeaponDataRef.GetDefaultObject()->ProjectileSpeed);
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
		SpawnFromPool(PrimaryWeaponDataRef.GetDefaultObject()->ProjectileClass);
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

	if (World != NULL && PrimaryWeaponDataRef != NULL)
	{
		float _randomFloat = FMath::FRandRange(0, PrimaryWeaponDataRef.GetDefaultObject()->Spread);
		const FRotator SpawnRotation = MyCharacter->GetControlRotation().Add(0, _randomFloat, _randomFloat);
		const FVector SpawnDirection = SpawnRotation.Vector();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : MyCharacter->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
	
		const FVector SpawnRange = SpawnLocation + SpawnDirection * PrimaryWeaponDataRef.GetDefaultObject()->HitScanRange;
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
	MyCharacter->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UWeaponComponent::ReloadComplete, PrimaryWeaponDataRef.GetDefaultObject()->ReloadTime, false);
}

void UWeaponComponent::ReloadComplete()
{
	CurrState = READY;
	AmmoConsumed = PrimaryWeaponDataRef.GetDefaultObject()->ClipSize;
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
