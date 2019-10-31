// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "PickableComponent.h"

// Sets default values for this component's properties
UPickableComponent::UPickableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPickableComponent::BeginPlay()
{
	Super::BeginPlay();

	if (NULL != GetOwner() && GetOwner()->GetRootComponent() != CollisionComp)
	{
		FVector position = GetOwner()->GetActorTransform().GetLocation();
		CollisionComp->SetWorldLocation(position);
		GetOwner()->SetRootComponent(CollisionComp);
		
	}

	if (CollisionComp != NULL)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UPickableComponent::OnOverlapBegin);
		CollisionComp->OnComponentEndOverlap.AddDynamic(this, &UPickableComponent::OnOverlapEnd);
	}
	// ...
}


// Called every frame
void UPickableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (canBePicked && target != nullptr)
		ShowPickPrompt(target);
}

//void UPickableComponent::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//}
//
//void UPickableComponent::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
//{
//}

void UPickableComponent::ShowPickPrompt(AActor * owner)
{
	UE_LOG(LogTemp, Log, TEXT("Prompt will be visible to %s"), *owner->GetName())
}

void UPickableComponent::OnPickUpTriggered()
{
}

void UPickableComponent::OnPickupComplete()
{
}

void UPickableComponent::SetCollisionComponent(UShapeComponent * component)
{
	CollisionComp = component;
}

