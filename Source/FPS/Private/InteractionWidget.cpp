// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "InteractionWidget.h"


UInteractionWidget::UInteractionWidget()
{
	bIsInputKeyPressed = false;
	bRotateToPlayer = true;
}

void UInteractionWidget::BeginPlay()
{
	Super::BeginPlay();

	if (TargetInteractor == nullptr)
	{
		TargetInteractor = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		DistanceToActivate *= DistanceToActivate;
	}

}

void UInteractionWidget::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TargetInteractor != nullptr)
	{
		if (FVector::DistSquared(GetOwner()->GetActorLocation(), TargetInteractor->GetActorLocation()) < DistanceToActivate)
		{
			SetVisibility(true);

		}
		else
		{
			bIsInputKeyPressed = false;		//Force kill the input when the player goes out of range
			SetVisibility(false);
		}

	}
}

void UInteractionWidget::InitWidget()
{
	Super::InitWidget();

	OnWidgetInitialized(Widget);
}

void UInteractionWidget::SetInputKeyState(bool state)
{
	bIsInputKeyPressed = state;
}

void UInteractionWidget::InteractionComplete()
{
	OnInteractionComplete();

	SetVisibility(false);

	TargetInteractor = nullptr;

	SetComponentTickEnabled(false);
}

