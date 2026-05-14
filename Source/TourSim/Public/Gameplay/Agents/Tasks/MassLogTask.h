// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MassSmartObjectRequest.h"
#include "MassStateTreeTypes.h"
#include "MassLogTask.generated.h"

/**
 * Instance data for FMassLogTask
 */
USTRUCT

()
struct TOURSIM_API FMassLogTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	FString Message = TEXT("Log Task");
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	FLinearColor Color = FLinearColor::Green;
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	float Duration = 1.0f;
	
	UPROPERTY(VisibleAnywhere, Category = Input)
	FMassSmartObjectCandidateSlots CandidateSlots;
};

/**
 * Simple debug task: prints a message to screen via UKismetSystemLibrary::PrintString
 */
USTRUCT(meta = (DisplayName = "Mass Log Task"))
struct TOURSIM_API FMassLogTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FMassLogTaskInstanceData;

protected:
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
