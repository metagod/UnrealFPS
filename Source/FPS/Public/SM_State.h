// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SM_State.generated.h"


UENUM()
enum class EStateMachineCompletionType : uint8
{
	NotAccepted,
	Accepted,
	Rejected,
	OutOfSteps UMETA (Hidden)
};

USTRUCT ()
struct FPS_API FStateMachineResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	EStateMachineCompletionType CompletionType;

	USM_State* FinalState;
	int32 DataIndex;
};

UCLASS()
class FPS_API USM_InputAtom : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY (EditAnywhere)
	FName Description;

};

UCLASS(EditInlineNew)
class FPS_API USM_Branch : public UDataAsset
{
	GENERATED_BODY()
public:

	virtual USM_State* TryBranch(const UObject* RefObect, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 &OutDataIndex);
protected:
	UPROPERTY (EditAnywhere)
	USM_State* DestinationState;

	UPROPERTY(EditAnywhere)
	uint32 bReverseInputTest : 1;


	/** List of acceptable inputs, Current input must be a part of it */
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> AcceptableInput;
};

/**
 State will define the what to do
 */
UCLASS()
class FPS_API USM_State : public UDataAsset
{
	GENERATED_BODY()
	
public:

	USM_State();

	virtual FStateMachineResult RunState(const UObject* RefObject, const TArray<USM_InputAtom*> &DataSource, int32 DataIndex = 0, int32 RemainingSteps = -1);

protected:
	virtual FStateMachineResult LoopState(const UObject* RefObject, const TArray<USM_InputAtom*> &DataSource, int32 DataIndex, int32 RemainingSteps);

	UPROPERTY (EditAnywhere)
	EStateMachineCompletionType CompletionType;
	
	UPROPERTY(EditAnywhere)
	uint32 bTerminateImmediately : 1;
	
	UPROPERTY(EditAnywhere)
	uint32 bLoopByDefault : 1;

	UPROPERTY(EditAnywhere)
	TArray<USM_Branch*> InstancedBranches;
};
