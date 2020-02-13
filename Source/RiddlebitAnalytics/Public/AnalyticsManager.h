#pragma once

#include "CoreMinimal.h"

//#include "Runtime/Online/HTTP/Public/Http.h"
#include "Http.h"
//#include "Dom/JsonObject.h"
//#include "Serialization/JsonReader.h"
//#include "Serialization/JsonSerializer.h"

#include "AnalyticsManager.generated.h"

UCLASS()

class RIDDLEBITANALYTICS_API UAnalyticsManager : public UObject
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintCallable, Category = "RiddlebitAnalytics")
	static void InitAnalytics();

	UFUNCTION(BlueprintCallable, Category = "RiddlebitAnalytics")
	static void PushAnalytics();

private:
	static FHttpModule* Http;
};

