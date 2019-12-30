// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "BaseWeaponActor.h"

void ABaseWeaponActor:: Tick (float DeltaTime)
{

}

void ABaseWeaponActor::Init(AFPSCharacter * owner)
{
	Super::Init(owner);
}

void ABaseWeaponActor::PrimaryUse()
{
	OnItemUsed();
}

void ABaseWeaponActor::SecondaryUse()
{
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

