// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UBaseStatClass : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseStatClass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float maxStatValue;

	float currentValue;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float minStatValue;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ModifyStat(float delta);

	FORCEINLINE float GetCurrentStatValue() const { return currentValue;  }
	FORCEINLINE float GetMaxValue () const { return maxStatValue;  }

};
