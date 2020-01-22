// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItemActor.h"
#include "BaseWeaponActor.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ABaseWeaponActor : public ABaseItemActor
{
	GENERATED_BODY()
protected:
	class UWeaponComponent* WeaponComponent;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init(class AFPSCharacter* owner) override;

	virtual void PrimaryUse() override;
	virtual void SecondaryUse() override;
	virtual void PickItem() override;
	virtual void EquipItem() override;
	virtual void DropItem() override;
	virtual void ThrowItem() override;
	virtual void InteractItem(AFPSCharacter* Owner) override;

	FVector GetMuzzleLocation();
};
