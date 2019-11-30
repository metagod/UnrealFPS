// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItemActor.generated.h"

UCLASS(abstract)
class FPS_API ABaseItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AFPSCharacter* MyOwner;

public:	

	UFUNCTION (BlueprintCallable, Category = "Item")
	FORCEINLINE class AFPSCharacter* GetItemOwner() { return MyOwner; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init(class AFPSCharacter* owner);

	virtual void OnPrimaryUse() PURE_VIRTUAL(ABaseItemActor::OnPrimaryUse, );
	virtual void OnSecondaryUse() PURE_VIRTUAL(ABaseItemActor::OnSecondaryUse, );
	virtual void OnPicked() PURE_VIRTUAL(ABaseItemActor::OnPicked, );
	virtual void OnEquipped() PURE_VIRTUAL(ABaseItemActor::OnEquipped, );
	virtual void OnDropped() PURE_VIRTUAL(ABaseItemActor::OnDropped, );
	virtual void OnThrow() PURE_VIRTUAL(ABaseItemActor::OnThrow, );

	UFUNCTION(BlueprintImplementableEvent, Category = "ItemState")
	void OnItemUsed();

	UFUNCTION(BlueprintImplementableEvent, Category = "ItemState")
	void OnItemUsedSecondary();

	UFUNCTION(BlueprintImplementableEvent, Category = "ItemState")
	void OnItemPicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "ItemState")
	void OnItemEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "ItemState")
	void OnItemDropped();

	UFUNCTION(BlueprintImplementableEvent, Category = "ItemState")
	void OnItemThrown();
};
