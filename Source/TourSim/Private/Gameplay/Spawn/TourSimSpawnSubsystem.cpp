// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Spawn/TourSimSpawnSubsystem.h"

#include "Gameplay/Spawn/AgentSpawnSourceBase.h"

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
	for (TWeakObjectPtr<AAgentSpawnSourceBase>& WeakSrc : Sources)
	{
		if (AAgentSpawnSourceBase* Src = WeakSrc.Get())
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

	// Round-robin distribution: allocate in passes until budget depleted
	// Each pass gives each source up to its local quota.
	int32 SafetyCounter = 0;
	const int32 SafetyMax = NumSources * 16; // prevents infinite loops if quotas misbehave

	while (BudgetLeft > 0 && SafetyCounter++ < SafetyMax)
	{
		bool bAnyAllocatedThisPass = false;

		for (int32 Step = 0; Step < NumSources && BudgetLeft > 0; ++Step)
		{
			const int32 Index = (RoundRobinIndex + Step) % NumSources;
			AAgentSpawnSourceBase* Src = Sources[Index].Get();
			if (!IsValid(Src))
			{
				continue;
			}

			const int32 Quota = Src->ComputeSpawnQuota(BudgetLeft);
			if (Quota <= 0)
			{
				continue;
			}

			// Reserve quota (actual consumption happens later: generator can fail validation)
			FSpawnRequest Req;
			Req.Source = Src;
			Req.Count  = Quota;
			CachedRequests.Add(Req);

			BudgetLeft -= Quota;
			bAnyAllocatedThisPass = true;
		}

		// Next frame starts from next source for fairness
		RoundRobinIndex = (RoundRobinIndex + 1) % NumSources;

		if (!bAnyAllocatedThisPass)
		{
			break; // no one can spawn anything right now
		}
	}
}

void UTourSimSpawnSubsystem::RegisterSource(AAgentSpawnSourceBase* Source)
{
	if (!IsValid(Source))
	{
		return;
	}
	
	for (const TWeakObjectPtr<AAgentSpawnSourceBase>& It : Sources)
	{
		if (It.Get() == Source)
		{
			return;
		}
	}
	
	Sources.Add(Source);
}

void UTourSimSpawnSubsystem::UnregisterSource(AAgentSpawnSourceBase* Source)
{
	if (!IsValid(Source))
	{
		return;
	}
	
	Sources.RemoveAll([Source](const TWeakObjectPtr<AAgentSpawnSourceBase>& It)
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
	Sources.RemoveAll([](const TWeakObjectPtr<AAgentSpawnSourceBase>& It)
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
