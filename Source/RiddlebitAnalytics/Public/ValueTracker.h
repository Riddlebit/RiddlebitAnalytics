#pragma once

#include "CoreMinimal.h"
#include "ValueTracker.generated.h"

UCLASS()
class RIDDLEBITANALYTICS_API UValueTracker : public UObject
{
	GENERATED_BODY()
public:
	void PrintValue();

	float* Value;
	FTimerHandle TimerHandle;
};