// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Spawn/MassAgentSpawnDataGenerator.h"

#include "MassEntityConfigAsset.h"
#include "MassSpawnLocationProcessor.h"
#include "Gameplay/Spawn/AgentSpawnSourceBase.h"
#include "Gameplay/Spawn/TourSimSpawnLog.h"
#include "Gameplay/Spawn/TourSimSpawnSubsystem.h"

void UMassAgentSpawnDataGenerator::Generate(
	UObject& QueryOwner, 
	TConstArrayView<FMassSpawnedEntityType> EntityTypes,
	int32 Count, 
	FFinishedGeneratingSpawnDataSignature& FinishedGeneratingSpawnPointsDelegate) const
{
	UE_LOG(LogTourSimSpawn, Warning,
	TEXT("[Generator] Requested=%d  EntityTypes=%d"),
	Count,
	EntityTypes.Num());
	
	UWorld* World = QueryOwner.GetWorld();
	if (!World || Count <= 0)
	{
		static const TArray<FMassEntitySpawnDataGeneratorResult> Empty;
		FinishedGeneratingSpawnPointsDelegate.ExecuteIfBound(Empty);
		return;
	}
	
	for (int32 i = 0; i < EntityTypes.Num(); ++i)
	{
		const FMassSpawnedEntityType& T = EntityTypes[i];

		const UObject* AssetObj = T.EntityConfig.Get(); // может быть null если не загружен
		const FString AssetName = AssetObj ? AssetObj->GetName() : TEXT("NULL");
		const FString AssetPath = T.EntityConfig.ToSoftObjectPath().ToString();

		UE_LOG(LogTourSimSpawn, Warning,
			TEXT("[Generator] EntityType[%d] ConfigName=%s  ConfigPath=%s"),
			i,
			*AssetName,
			*AssetPath);
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
	
	UE_LOG(LogTourSimSpawn, Warning,
	TEXT("[Generator] AllowedByBudget=%d"),
	AllowedCount);
	
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
	
	// Подготовка SpawnData для каждого slice
	struct FSliceBuild
	{
		FMassEntitySpawnDataGeneratorResult* Result = nullptr;
		FMassTransformsSpawnData SpawnData;
		int32 Remaining = 0;
	};

	TArray<FSliceBuild> Slices;
	Slices.Reserve(Results.Num());

	for (FMassEntitySpawnDataGeneratorResult& R : Results)
	{
		if (R.NumEntities <= 0)
		{
			continue;
		}

		FSliceBuild& S = Slices.AddDefaulted_GetRef();
		S.Result = &R;
		S.Remaining = R.NumEntities;

		S.SpawnData.bRandomize = bRandomizeOutputTransforms;
		S.SpawnData.Transforms.Reserve(R.NumEntities);
	}

	// Итератор по Requests: достаём валидные точки из источников
	int32 RequestIndex = 0;

	auto TryPullOneTransform = [&](FTransform& OutXform) -> bool
	{
		while (RequestIndex < Requests.Num())
		{
			FSpawnRequest& Request = Requests[RequestIndex];
			AAgentSpawnSourceBase* Source = Request.Source.Get();

			if (!IsValid(Source) || Request.Count <= 0)
			{
				++RequestIndex;
				continue;
			}

			FTransform Xform;
			if (!TryBuildValidatedTransform(*World, QueryOwner, Source, Xform))
			{
				Request.Count = 0;
				++RequestIndex;
				continue;
			}

			if (!Source->TryConsumeOne())
			{
				Request.Count = 0;
				++RequestIndex;
				continue;
			}

			Request.Count -= 1;
			OutXform = Xform;
			return true;
		}

		return false;
	};

	// Round-robin раздача точек по типам (гарантирует присутствие обоих, если хотя бы 2 точки есть)
	const TSubclassOf<UMassProcessor> ProcessorClass =
		SpawnDataProcessorClass ? *SpawnDataProcessorClass : UMassSpawnLocationProcessor::StaticClass();

	int32 Safety = 0;
	const int32 SafetyMax = AllowedCount * 4;

	int32 SliceIndex = 0;
	while (Safety++ < SafetyMax)
	{
		bool bAnyRemaining = false;

		for (int32 Iter = 0; Iter < Slices.Num(); ++Iter)
		{
			FSliceBuild& S = Slices[(SliceIndex + Iter) % Slices.Num()];
			if (S.Remaining <= 0)
			{
				continue;
			}

			bAnyRemaining = true;

			FTransform Xform;
			if (!TryPullOneTransform(Xform))
			{
				// точек больше нет вообще
				SliceIndex = (SliceIndex + Iter) % Slices.Num();
				Safety = SafetyMax; // break outer
				break;
			}

			S.SpawnData.Transforms.Add(Xform);
			S.Remaining -= 1;
		}

		if (!bAnyRemaining)
		{
			break;
		}

		SliceIndex = (SliceIndex + 1) % Slices.Num();
	}

	// Финализация Result’ов
	for (FSliceBuild& S : Slices)
	{
		const int32 Produced = S.SpawnData.Transforms.Num();

		S.Result->NumEntities = Produced;
		if (Produced > 0)
		{
			S.Result->SpawnDataProcessor = ProcessorClass;
			S.Result->PostSpawnProcessors.Reset();
			S.Result->SpawnData = FInstancedStruct::Make<FMassTransformsSpawnData>(MoveTemp(S.SpawnData));
		}
	}

	FinishedGeneratingSpawnPointsDelegate.ExecuteIfBound(Results);
}

bool UMassAgentSpawnDataGenerator::TryBuildValidatedTransform(UWorld& World, UObject& QueryOwner, AActor* SourceActor,
	FTransform& OutTransform) const
{
	AAgentSpawnSourceBase* Source = Cast<AAgentSpawnSourceBase>(SourceActor);
	
	if (!Cast<AAgentSpawnSourceBase>(SourceActor))
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
			UE_LOG(LogTourSimSpawn, Warning,
				TEXT("[Generator] Validation failed for source %s"),
				*SourceActor->GetName());

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
	AAgentSpawnSourceBase* Source = Cast<AAgentSpawnSourceBase>(SourceActor);
	if (!IsValid(Source))
	{
		return false;
	}
	
	FVector P = InOutTransform.GetLocation();
	const float SourceZ = Source->GetActorLocation().Z;
	P.Z = SourceZ + CapsuleHalfHeight + GroundZOffset;
	InOutTransform.SetLocation(P);

	constexpr int32 MaxLiftSteps = 10;
	constexpr float LiftStepCm = 20.f;

	for (int32 Step = 0; Step <= MaxLiftSteps; ++Step)
	{
		if (CapsuleOverlapTest(World, InOutTransform, QueryOwner, SourceActor))
		{
			return true;
		}

		P = InOutTransform.GetLocation();
		P.Z += LiftStepCm;
		InOutTransform.SetLocation(P);
	}

	UE_LOG(LogTourSimSpawn, Warning, TEXT("[Validate] CapsuleOverlap FAIL after lift. Source=%s Pos=%s"),
		SourceActor ? *SourceActor->GetName() : TEXT("None"),
		*InOutTransform.GetLocation().ToString());
	return false;
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
