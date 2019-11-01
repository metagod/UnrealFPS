// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "HealthStat.h"

UHealthStat::UHealthStat()
{
	
}

void UHealthStat::OnDamageTaken_Implementation(AActor* damageDealer, float damage)
{
	ModifyStat(damage);
}