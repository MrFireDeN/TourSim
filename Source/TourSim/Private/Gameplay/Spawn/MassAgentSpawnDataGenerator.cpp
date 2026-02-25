// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Spawn/MassAgentSpawnDataGenerator.h"

#include "MassSpawnLocationProcessor.h"
#include "NavigationSystem.h"
#include "Gameplay/Spawn/AgentSpawnSourceBase.h"
#include "Gameplay/Spawn/TourSimSpawnSubsystem.h"
#include "GeometryCollection/GeometryCollectionParticlesData.h"

void UMassAgentSpawnDataGenerator::Generate(
	UObject& QueryOwner, 
	TConstArrayView<FMassSpawnedEntityType> EntityTypes,
	int32 Count, 
	FFinishedGeneratingSpawnDataSignature& FinishedGeneratingSpawnPointsDelegate) const
{
	UWorld* World = QueryOwner.GetWorld();
	if (!World || Count <= 0)
	{
		static const TArray<FMassEntitySpawnDataGeneratorResult> Empty;
		FinishedGeneratingSpawnPointsDelegate.ExecuteIfBound(Empty);
		return;
	}
	
	UTourSimSpawnSubsystem* SpawnSubsystem = World->GetSubsystem<UTourSimSpawnSubsystem>();
	if (!SpawnSubsystem)
	{
		static const TArray<FMassEntitySpawnDataGeneratorResult> Empty;
		FinishedGeneratingSpawnPointsDelegate.ExecuteIfBound(Empty);
		return;
	}
	
	TArray<FSpawnRequest> Requests;
	SpawnSubsystem->BuildSpawnRequests(Requests);
	
	int32 BudgetAllowed = 0;
	for (const auto& Source : Requests)
	{
		BudgetAllowed += FMath::Max(0, Source.Count);
	}
	
	const int32 AllowedCount = FMath::Min(Count, BudgetAllowed);
	if (AllowedCount <= 0)
	{
		static const TArray<FMassEntitySpawnDataGeneratorResult> Empty;
		FinishedGeneratingSpawnPointsDelegate.ExecuteIfBound(Empty);
		return;
	}
	
	TArray<FMassEntitySpawnDataGeneratorResult> Results;
	BuildResultsFromEntityTypes(AllowedCount, EntityTypes, Results);
	
	if (Results.IsEmpty())
	{
		static const TArray<FMassEntitySpawnDataGeneratorResult> Empty;
		FinishedGeneratingSpawnPointsDelegate.ExecuteIfBound(Empty);
		return;
	}
	
	TArray<FTransform> TransformPool;
	TransformPool.Reserve(AllowedCount);
	
	int32 RequestIndex = 0;
	
	while (TransformPool.Num() < AllowedCount && RequestIndex < Results.Num())
	{
		FSpawnRequest& Request = Requests[RequestIndex];
		AAgentSpawnSourceBase* Source = Request.Source.Get();
		
		if (!IsValid(Source) || Request.Count <= 0)
		{
			++RequestIndex;
			continue;
		}
		
		const int32 Remaining = AllowedCount - TransformPool.Num();
		const int32 WantFromThisSource = FMath::Min(Request.Count, Remaining);
		
		for (int32 Index = 0; Index < WantFromThisSource; ++Index)
		{
			FTransform Xform;
			if (!TryBuildValidatedTransform(*World, QueryOwner, Source, Xform))
			{
				Request.Count = 0;
				break;
			}
			
			if (!Source->TryConsumeOne())
			{
				Request.Count = 0;
				break;
			}
			
			TransformPool.Add(Xform);
			Request.Count -= 1;
			
			if (TransformPool.Num() >= AllowedCount)
			{
				break;
			}
		}
		
		++RequestIndex;
	}
	
	const TSubclassOf<UMassProcessor> ProcessorClass = 
		SpawnDataProcessorClass ? *SpawnDataProcessorClass : UMassSpawnLocationProcessor::StaticClass();
	
	int32 PoolOffset = 0;
	
	for (FMassEntitySpawnDataGeneratorResult& Result : Results)
	{
		const int32 RemainingInPool = TransformPool.Num() - PoolOffset;
		const int32 ActualCount = FMath::Min(Result.NumEntities, FMath::Max(0, RemainingInPool));
		
		if (ActualCount <= 0)
		{
			Result.NumEntities = 0;
			continue;
		}
		
		FMassTransformsSpawnData SpawnData;
		SpawnData.bRandomize = bRandomizeOutputTransforms;
		SpawnData.Transforms.Reserve(ActualCount);
		
		for (int32 Index = 0; Index < ActualCount; ++Index)
		{
			SpawnData.Transforms.Add(TransformPool[Index + PoolOffset]);
		}
		
		PoolOffset += ActualCount;
		
		Result.NumEntities = ActualCount;
		Result.SpawnDataProcessor = ProcessorClass;
		Result.PostSpawnProcessors.Reset();
		Result.SpawnData = FInstancedStruct::Make<FMassTransformsSpawnData>(MoveTemp(SpawnData));
	}
	
	FinishedGeneratingSpawnPointsDelegate.ExecuteIfBound(Results);
}

bool UMassAgentSpawnDataGenerator::TryBuildValidatedTransform(UWorld& World, UObject& QueryOwner, AActor* SourceActor,
	FTransform& OutTransform) const
{
	AAgentSpawnSourceBase* Source = Cast<AAgentSpawnSourceBase>(SourceActor);
	
	if (Cast<AAgentSpawnSourceBase>(SourceActor))
	{
		return false;
	}
	
	for (int32 Attempt = 0; Attempt < MaxPointAttemptsPerEntity; ++Attempt)
	{
		FTransform Candidate;
		if (!Source->GetCandidateTransform(Candidate))
		{
			return false;
		}
		
		if (!ValidatePoint(World, QueryOwner, SourceActor, Candidate))
		{
			continue;
		}
		
		OutTransform = Candidate;
		return true;
	}
	
	return false;
}

bool UMassAgentSpawnDataGenerator::ValidatePoint(UWorld& World, UObject& QueryOwner, AActor* SourceActor,
	FTransform& InOutTransform) const
{
	FVector Pos = InOutTransform.GetLocation();
	
	
	if (!ProjectToNavmesh(World, Pos))
	{
		return false;
	}
	
	if (!GroundTrace(World, InOutTransform, SourceActor))
	{
		return false;
	}
	
	if (!CapsuleOverlapTest(World, InOutTransform, QueryOwner, SourceActor))
	{
		return false;
	}
	
	return true;
}

bool UMassAgentSpawnDataGenerator::ProjectToNavmesh(UWorld& World, FVector& InOutPosition) const
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(&World);
	if (!NavSystem)
	{
		return false;
	}
	
	FNavLocation Projected;
	if (!NavSystem->ProjectPointToNavigation(InOutPosition, Projected, NavQueryExtent))
	{
		return false;
	}
	
	InOutPosition = Projected.Location;
	return true;
}

bool UMassAgentSpawnDataGenerator::GroundTrace(UWorld& World, FTransform& InOutTransform, AActor* SourceActor) const
{
	const FVector Base	= InOutTransform.GetLocation();
	const FVector Start = Base + FVector(0, 0, TraceHeightUp);
	const FVector End	= Base - FVector(0, 0, TraceHeightDown);
	
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(TourSimSpawn_GroundTrace), false);
	
	if (SourceActor)
	{
		Params.AddIgnoredActor(SourceActor);
	}
	
	if (!World.LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
	{
		return false;
	}
	
	FVector Adjusted = Hit.ImpactPoint;
	Adjusted.Z += CapsuleHalfHeight + GroundZOffset;
	InOutTransform.SetLocation(Adjusted);
	return true;
}

bool UMassAgentSpawnDataGenerator::CapsuleOverlapTest(UWorld& World, const FTransform& Transform, UObject& QueryOwner,
	AActor* SourceActor) const
{
	const FCollisionShape Capsule = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(TourSimSpawn_CapsuleOverlap), false);
	
	if (AActor* OwnerActor = Cast<AActor>(&QueryOwner))
	{
		Params.AddIgnoredActor(OwnerActor);
	}
	if (SourceActor)
	{
		Params.AddIgnoredActor(SourceActor);
	}
	
	return !World.OverlapBlockingTestByChannel(
		Transform.GetLocation(), 
		Transform.GetRotation(), 
		ECC_Pawn, 
		Capsule, 
		Params
	);
}
