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
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init(class AFPSCharacter* owner) override;

	virtual void OnPrimaryUse() override;
	virtual void OnSecondaryUse() override;
	virtual void OnPicked() override;
	virtual void OnEquipped() override;
	virtual void OnDropped() override;
	virtual void OnThrow() override;
};
