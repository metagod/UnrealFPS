// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS.h"
#include "SM_State.h"

USM_State::USM_State()
{
	bLoopByDefault = true;
}

FStateMachineResult USM_State::RunState(const UObject* RefObject, const TArray<USM_InputAtom*> &DataSource, int32 DataIndex, int32 RemainingSteps)
{
	bool bEndNow = bTerminateImmediately || !DataSource.IsValidIndex(DataIndex);

	if (RemainingSteps && !bEndNow)
	{
		USM_State* DestinationState = nullptr;
		int32 DestinationDataIndex = DataIndex;

		for (int32 i = 0; i < InstancedBranches.Num(); ++i)
		{
			if (InstancedBranches[i])
			{
				DestinationState = InstancedBranches[i]->TryBranch(RefObject, DataSource, DataIndex, DestinationDataIndex);
				if (DestinationState)
				{
					DestinationState->RunState(RefObject, DataSource, DestinationDataIndex, RemainingSteps - 1);
				}
			}

			if (bLoopByDefault)
			{
				return LoopState(RefObject, DataSource, DataIndex, RemainingSteps);
			}

			bEndNow = true;
		}
	}

	FStateMachineResult Result;
	Result.FinalState = this;
	Result.DataIndex = DataIndex;
	Result.CompletionType = bEndNow ? CompletionType : EStateMachineCompletionType::OutOfSteps;

	return Result;
}

FStateMachineResult USM_State::LoopState(const UObject * RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps)
{
	return RunState(RefObject, DataSource, DataIndex + 1, RemainingSteps -1);
}

USM_State* USM_Branch::TryBranch(const UObject* RefObect, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 &OutDataIndex)
{
	OutDataIndex = DataIndex;

	if (!AcceptableInput.Num() ||
		DataSource.IsValidIndex(DataIndex) && AcceptableInput.Contains(DataSource[DataIndex]))
	{
		++OutDataIndex;
		return bReverseInputTest ? nullptr : DestinationState;
	}

	return bReverseInputTest ? DestinationState : nullptr;
}