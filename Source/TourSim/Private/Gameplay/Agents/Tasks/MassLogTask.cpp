// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gameplay/Agents/Tasks/MassLogTask.h"

#include "StateTreeExecutionContext.h"
#include "Kismet/KismetSystemLibrary.h"

EStateTreeRunStatus FMassLogTask::EnterState(FStateTreeExecutionContext& Context,
                                             const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	UWorld* World = Context.GetWorld();
	if (!World)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	UKismetSystemLibrary::PrintString(
		World, 
		InstanceData.Message, 
		true, 
		true, 
		InstanceData.Color, 
		InstanceData.Duration
	);
	
	UKismetSystemLibrary::PrintString(
		World, 
		FString::Printf(TEXT("Num slots: %d"), InstanceData.CandidateSlots.NumSlots), 
		true, 
		true, 
		InstanceData.Color, 
		InstanceData.Duration
	);
	
	return EStateTreeRunStatus::Succeeded;
}
