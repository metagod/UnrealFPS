// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	UPROPERTY(EditAnywhere, Category = "InteractableItem")
	UShapeComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractableItem")
	AActor* target;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractableItem")
	class UWidget* PickWidget;

	// Called when the game starts
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintPure, Category = "InteractableItem")
	virtual bool CanInteractWithItem();

	UFUNCTION(BlueprintImplementableEvent, Category = "InteractableItem")
	void ShowInteractPrompt(class AActor* owner);

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Category = "InteractableItem")
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "InteractableItem")
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable, Category = "InteractableItem")
	void SetCollisionComponent(UShapeComponent* component);

};
