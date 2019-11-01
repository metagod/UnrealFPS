// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStatClass.h"
#include "DamageReceiverInterface.h"
#include "HealthStat.generated.h"
/**
 * 
 */
UCLASS()
class FPS_API UHealthStat : public UBaseStatClass, public IDamageReceiverInterface
{
	GENERATED_BODY()
	
public:

	UHealthStat();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void OnDamageTaken(AActor* damageDealer, float damageDone);

	virtual void OnDamageTaken_Implementation(AActor* damageDealer, float damageDone) override;
};
