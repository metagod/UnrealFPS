// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "BaseItemActor.h"

// Sets default values
ABaseItemActor::ABaseItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItemActor::Init(AFPSCharacter * owner)
{
	if (nullptr != owner)
		MyOwner = owner;
}

void ABaseItemActor::OnItemInteracted_Implementation()
{
}

void ABaseItemActor::OnItemUsed_Implementation()
{}

void ABaseItemActor::OnItemDropped_Implementation()
{}

void ABaseItemActor::OnItemEquipped_Implementation()
{}

void ABaseItemActor::OnItemPicked_Implementation()
{}

void ABaseItemActor::OnItemThrown_Implementation()
{}

void ABaseItemActor::OnItemUsedSecondary_Implementation()
{}

