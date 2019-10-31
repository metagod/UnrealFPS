// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickableComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UPickableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickableComponent();

	UPROPERTY(EditAnywhere, Category = "PickableItem")
	UShapeComponent* CollisionComp;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PickableItem")
	bool canBePicked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickableItem")
	AActor* target;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "PickableItem")
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "PickableItem")
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "PickableItem")
	void TestBPImpFunc(float doSomething);

	UFUNCTION(BlueprintCallable, Category = "PickableItem")
	void ShowPickPrompt(class AActor* owner);

	UFUNCTION(BlueprintCallable, Category = "PickableItem")
	virtual void OnPickUpTriggered();

	UFUNCTION(BlueprintCallable, Category = "PickableItem")
	virtual void OnPickupComplete();

	UFUNCTION(BlueprintCallable, Category = "PickableItem")
	void SetCollisionComponent(UShapeComponent* component);
};
