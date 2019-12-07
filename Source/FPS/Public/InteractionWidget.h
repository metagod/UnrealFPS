// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = "UserInterface", hidecategories = (Object, Activation, "Components|Activation", Sockets, Base, Lighting, LOD, Mesh), editinlinenew, meta = (BlueprintSpawnableComponent))
class FPS_API UInteractionWidget : public UWidgetComponent
{
	GENERATED_BODY()

	bool bIsInputKeyPressed;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bRotateToPlayer;
public:
	
	UInteractionWidget();

	/** This will be set to Player by default if no other value is assigned to it*/
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	AFPSCharacter* TargetInteractor;

	/** Range at which the prompt with become visible, its the min distance between this actor and target interactor */
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float DistanceToActivate;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void InitWidget() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnWidgetInitialized(UUserWidget* myWidget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionComplete();

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool GetInputKeyState() const { return bIsInputKeyPressed; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInputKeyState(bool state);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractionComplete();

};
