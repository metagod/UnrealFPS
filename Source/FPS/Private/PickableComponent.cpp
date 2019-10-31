// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "PickableComponent.h"

// Sets default values for this component's properties
UPickableComponent::UPickableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if (CollisionComp == nullptr)
	{
		UBoxComponent* collider = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
		collider->SetBoxExtent(FVector(5, 5, 5));
		collider->SetRelativeLocation(FVector::ZeroVector);
		collider->SetWorldLocation(FVector::ZeroVector);
		collider->SetCollisionProfileName(TEXT("Trigger"));
		collider->OnComponentBeginOverlap.AddDynamic(this, &UPickableComponent::OnOverlapBegin);
		collider->OnComponentEndOverlap.AddDynamic(this, &UPickableComponent::OnOverlapEnd);
		CollisionComp = collider;
		collider = NULL;
	}
	// ...
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
	// ...
}


// Called every frame
void UPickableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (canBePicked && target != nullptr)
		ShowPickPrompt(target);

}

void UPickableComponent::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void UPickableComponent::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}

void UPickableComponent::ShowPickPrompt(AActor * owner)
{
}

void UPickableComponent::SetCollisionComponent(UShapeComponent * component)
{
	CollisionComp = component;
}

