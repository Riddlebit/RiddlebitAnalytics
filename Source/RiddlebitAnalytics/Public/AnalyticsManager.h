#pragma once

#include "CoreMinimal.h"

//#include "Runtime/Online/HTTP/Public/Http.h"
#include "Http.h"


#include "Misc/DateTime.h" 
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "AnalyticsManager.generated.h"

UCLASS()
class RIDDLEBITANALYTICS_API UAnalyticsManager : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "RiddlebitAnalytics")
	static void InitAnalytics();

	UFUNCTION(BlueprintCallable, Category = "RiddlebitAnalytics")
	static void RegisterAnalytics(UAnalyticsData* Data);

	UFUNCTION(BlueprintCallable, Category = "RiddlebitAnalytics")
	static void PushAnalytics();

	TArray<UAnalyticsData*> Buffer;

private:
	static FHttpModule* Http;
	static UAnalyticsManager* Instance;
};

UCLASS(Blueprintable, BlueprintType)
class RIDDLEBITANALYTICS_API UAnalyticsData : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "RiddlebitAnalytics")
	static UAnalyticsData* MakeAnalyticsData();
	
	UFUNCTION(BlueprintPure, Category = "RiddlebitAnalytics")
	UAnalyticsData* AddField(FString Key, FString Data);

	FString ToJsonString();

	UAnalyticsData();

private:
	FString TimeStamp;

	TMap<FString, FString> Fields;
};