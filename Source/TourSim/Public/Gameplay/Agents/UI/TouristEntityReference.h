// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "UObject/Object.h"
#include "TouristEntityReference.generated.h"

/**
 * Blueprint-safe wrapper around FMassEntityHandle.
 */
UCLASS(BlueprintType)
class TOURSIM_API UTouristEntityReference : public UObject
{
	GENERATED_BODY()
	
public:

	void Initialize(FMassEntityHandle InHandle)
	{
		EntityHandle = InHandle;
	}

	const FMassEntityHandle& GetHandle() const
	{
		return EntityHandle;
	}

private:
	FMassEntityHandle EntityHandle;
};
