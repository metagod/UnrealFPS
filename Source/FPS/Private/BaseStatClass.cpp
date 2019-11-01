// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "BaseStatClass.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UBaseStatClass::UBaseStatClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseStatClass::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseStatClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseStatClass::ModifyStat(float delta)
{
	if (currentValue > 0)
	{
		currentValue -= delta;
		currentValue = FMath::Clamp(currentValue, minStatValue, maxStatValue);
	}
}

