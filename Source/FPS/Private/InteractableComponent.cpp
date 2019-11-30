// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "InteractableComponent.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UInteractableComponent::SetCollisionComponent(UShapeComponent * component)
{
	CollisionComp = component;

	if (NULL != GetOwner() && GetOwner()->GetRootComponent() != CollisionComp)
	{
		FVector position = GetOwner()->GetActorTransform().GetLocation();
		CollisionComp->SetWorldLocation(position);
		GetOwner()->SetRootComponent(CollisionComp);
	}
}

bool UInteractableComponent::CanInteractWithItem()
{
	if (target == nullptr)
		return false;

	FVector actorForwardVector = target->GetActorForwardVector();
	FVector actorToItemVector = GetOwner()->GetActorLocation() - target->GetActorLocation();

	actorForwardVector.Normalize();
	actorToItemVector.Normalize();

	float dotValue = FVector::DotProduct(actorForwardVector, actorToItemVector);

	return dotValue > 0.5f && dotValue < 1.0f;

}

// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	if (CollisionComp != NULL)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapBegin);
		CollisionComp->OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapEnd);
	}

}



void UInteractableComponent::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (target != nullptr)
		return;

	if (Cast<AFPSCharacter>(OtherActor) != nullptr)
	{
		target = OtherActor;
	}

}

void UInteractableComponent::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (target != nullptr && target == OtherActor)
	{
		target = nullptr;
	}
}

// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (target != nullptr && CanInteractWithItem())
		ShowInteractPrompt(target);
	// ...
}

