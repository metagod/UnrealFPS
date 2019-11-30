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

void ABaseWeaponActor::OnPrimaryUse()
{
	OnItemUsed();
}

void ABaseWeaponActor::OnSecondaryUse()
{
	OnItemUsedSecondary();
}

void ABaseWeaponActor::OnPicked()
{
	OnItemPicked();
}

void ABaseWeaponActor::OnEquipped()
{
	OnItemEquipped();
}

void ABaseWeaponActor::OnDropped()
{
	OnItemDropped();
}

void ABaseWeaponActor::OnThrow()
{
	OnItemThrown();
}
