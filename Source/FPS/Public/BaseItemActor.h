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

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Item")
	FRotator RelativeRoationOnEquip;

public:	

	UFUNCTION (BlueprintPure, Category = "Item")
	FORCEINLINE class AFPSCharacter* GetItemOwner() { return MyOwner; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init(AFPSCharacter* owner);

	UFUNCTION(BlueprintCallable, Category = "ItemBehaviour")
	virtual void PrimaryUse() PURE_VIRTUAL(ABaseItemActor::PrimaryUse, );
	
	UFUNCTION(BlueprintCallable, Category = "ItemBehaviour")
	virtual void SecondaryUse() PURE_VIRTUAL(ABaseItemActor::SecondaryUse, );
	
	UFUNCTION(BlueprintCallable, Category = "ItemBehaviour")
	virtual void PickItem() PURE_VIRTUAL(ABaseItemActor::PickItem, );
	
	UFUNCTION(BlueprintCallable, Category = "ItemBehaviour")
	virtual void EquipItem() PURE_VIRTUAL(ABaseItemActor::EquipItem, );
	
	UFUNCTION(BlueprintCallable, Category = "ItemBehaviour")
	virtual void DropItem() PURE_VIRTUAL(ABaseItemActor::DropItem, );
	
	UFUNCTION(BlueprintCallable, Category = "ItemBehaviour")
	virtual void ThrowItem() PURE_VIRTUAL(ABaseItemActor::ThrowItem, );

	UFUNCTION (BlueprintCallable, Category = "ItemBehaviour")
	virtual void InteractItem(AFPSCharacter* newOwner) PURE_VIRTUAL(ABaseItemActor::InteractItem, );

	UFUNCTION(BlueprintNativeEvent, Category = "ItemState")
	void OnItemInteracted();
	virtual void OnItemInteracted_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "ItemState")
	void OnItemUsed();
	virtual void OnItemUsed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "ItemState")
	void OnItemUsedSecondary();
	virtual void OnItemUsedSecondary_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "ItemState")
	void OnItemPicked();
	virtual void OnItemPicked_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "ItemState")
	void OnItemEquipped();
	virtual void OnItemEquipped_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "ItemState")
	void OnItemDropped();
	virtual void OnItemDropped_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "ItemState")
	void OnItemThrown();
	virtual void OnItemThrown_Implementation();

	UFUNCTION (BlueprintPure, Category = "Item")
	FORCEINLINE FRotator GetRelativeRotation() const { return RelativeRoationOnEquip; }
};
