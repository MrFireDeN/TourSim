// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Spawn/TourSimSpawnSubsystem.h"

#include "Gameplay/Spawn/TouristSpawnSourceBase.h"
#include "Gameplay/Spawn/TourSimSpawnLog.h"

void UTourSimSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Sources.Reserve(16);
	CachedRequests.Reserve(16);
	RoundRobinIndex = 0;
}

void UTourSimSpawnSubsystem::Deinitialize()
{
	Sources.Reset();
	CachedRequests.Reset();
	Super::Deinitialize();
}

void UTourSimSpawnSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CompactSources();
	
	// 1) Each source accumulates demand (λ -> queue)
	for (TWeakObjectPtr<ATouristSpawnSourceBase>& WeakSrc : Sources)
	{
		if (ATouristSpawnSourceBase* Src = WeakSrc.Get())
		{
			Src->AccumulateDemand(DeltaTime);
		}
	}

	// 2) Build per-frame spawn plan (cached)
	CachedRequests.Reset();

	int32 BudgetLeft = GlobalMaxSpawnPerTick;
	const int32 NumSources = Sources.Num();
	if (BudgetLeft <= 0 || NumSources == 0)
	{
		return;
	}

	TMap<TWeakObjectPtr<ATouristSpawnSourceBase>, int32> AllocatedPerSource;
	
	int32 SafetyCounter = 0;
	const int32 SafetyMax = NumSources * 16; // prevents infinite loops if quotas misbehave

	while (BudgetLeft > 0 && SafetyCounter++ < SafetyMax)
	{
		bool bAnyAllocatedThisPass = false;

		for (int32 Step = 0; Step < NumSources && BudgetLeft > 0; ++Step)
		{
			const int32 Index = (RoundRobinIndex + Step) % NumSources;
			ATouristSpawnSourceBase* Src = Sources[Index].Get();
			if (!IsValid(Src))
			{
				continue;
			}

			const int32 Quota = Src->ComputeSpawnQuota(BudgetLeft);
			if (Quota <= 0)
			{
				continue;
			}

			AllocatedPerSource.FindOrAdd(Src) += Quota;
			BudgetLeft -= Quota;
			bAnyAllocatedThisPass = true;
		}

		RoundRobinIndex = (RoundRobinIndex + 1) % NumSources;

		if (!bAnyAllocatedThisPass)
		{
			break;
		}
	}
	
	for (const TPair<TWeakObjectPtr<ATouristSpawnSourceBase>, int32>& Kvp : AllocatedPerSource)
	{
		if (!Kvp.Key.IsValid() || Kvp.Value <= 0)
		{
			continue;
		}

		FSpawnRequest Req;
		Req.Source = Kvp.Key;
		Req.Count = Kvp.Value;
		CachedRequests.Add(Req);
	}
	
	UE_LOG(LogTourSimSpawn, Warning,
	TEXT("[SpawnSubsystem] Sources=%d  Requests=%d  GlobalBudget=%d"),
	Sources.Num(),
	CachedRequests.Num(),
	GlobalMaxSpawnPerTick);

	for (const FSpawnRequest& Req : CachedRequests)
	{
		if (ATouristSpawnSourceBase* Src = Req.Source.Get())
		{
			UE_LOG(LogTourSimSpawn, Warning,
				TEXT("   -> %s  Count=%d  PendingQueue=%.2f"),
				*Src->GetName(),
				Req.Count,
				Src->GetPendingQueue());
		}
	}
}

void UTourSimSpawnSubsystem::RegisterSource(ATouristSpawnSourceBase* Source)
{
	if (!IsValid(Source))
	{
		return;
	}
	
	for (const TWeakObjectPtr<ATouristSpawnSourceBase>& It : Sources)
	{
		if (It.Get() == Source)
		{
			return;
		}
	}
	
	Sources.Add(Source);
}

void UTourSimSpawnSubsystem::UnregisterSource(ATouristSpawnSourceBase* Source)
{
	if (!IsValid(Source))
	{
		return;
	}
	
	Sources.RemoveAll([Source](const TWeakObjectPtr<ATouristSpawnSourceBase>& It)
	{
		return !It.IsValid() || It.Get() == Source;
	});
	
	if (Sources.Num() == 0)
	{
		RoundRobinIndex = 0;
	}
	else
	{
		RoundRobinIndex = RoundRobinIndex % Sources.Num();
	}
}

void UTourSimSpawnSubsystem::BuildSpawnRequests(TArray<FSpawnRequest>& OutRequests)
{
	// Generator calls this; provide the plan computed on Tick()
	OutRequests = CachedRequests;
}

void UTourSimSpawnSubsystem::CompactSources()
{
	Sources.RemoveAll([](const TWeakObjectPtr<ATouristSpawnSourceBase>& It)
	{
		return !It.IsValid();
	});

	if (Sources.Num() == 0)
	{
		RoundRobinIndex = 0;
	}
	else
	{
		RoundRobinIndex = RoundRobinIndex % Sources.Num();
	}
}
