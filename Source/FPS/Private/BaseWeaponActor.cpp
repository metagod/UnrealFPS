// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "BaseWeaponActor.h"
#include "WeaponComponent.h"



void ABaseWeaponActor:: Tick (float DeltaTime)
{

}

void ABaseWeaponActor::Init(AFPSCharacter * owner)
{
	Super::Init(owner);

	if (WeaponComponent == nullptr)
	{
		WeaponComponent = this->FindComponentByClass<UWeaponComponent>();
	}

	WeaponComponent->Init(owner);
}

void ABaseWeaponActor::PrimaryUse()
{
	WeaponComponent->FirePrimaryPressed();
	OnItemUsed();
}

void ABaseWeaponActor::SecondaryUse()
{
	WeaponComponent->FireSecondaryPressed();
	OnItemUsedSecondary();
}

void ABaseWeaponActor::PickItem()
{
	SetActorHiddenInGame(true);

	OnItemPicked();
}

void ABaseWeaponActor::EquipItem()
{
	if (MyOwner != nullptr)
		MyOwner->EquipThisItem(this);

	OnItemEquipped();
}

void ABaseWeaponActor::DropItem()
{
	OnItemDropped();

	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	
}

void ABaseWeaponActor::ThrowItem()
{
	OnItemThrown();
}

void ABaseWeaponActor::InteractItem(AFPSCharacter * newOwner)
{
	if (newOwner)
	{
		Init(newOwner);

		OnItemInteracted();
	}
}

FVector ABaseWeaponActor::GetMuzzleLocation()
{
	return GetActorLocation() + FVector(0, 0, 3);
}

