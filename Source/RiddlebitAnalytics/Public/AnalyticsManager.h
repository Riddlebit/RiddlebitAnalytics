#pragma once

#include "CoreMinimal.h"

#include "Http.h"
#include "Misc/DateTime.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/ObjectWriter.h" 
#include "Containers/UnrealString.h"

#include "AnalyticTypes.h"
#include "ValueTracker.h"
#include "AnalyticsManager.generated.h"

UCLASS()
class RIDDLEBITANALYTICS_API UAnalyticsManager : public UObject
{
	GENERATED_BODY()

public:

	static void InitAnalytics(FString Ip, FString Port);

	UFUNCTION(BlueprintCallable, Category = "RiddlebitAnalytics")
	static void RegisterAnalytics(UAnalyticsData* Data);

	UFUNCTION(BlueprintCallable, Category = "RiddlebitAnalytics")
	static void RegisterFloatTracker(UPARAM(ref) float& FloatPointer);

	UFUNCTION(BlueprintCallable, Category = "RiddlebitAnalytics")
	static void PushAnalytics();

	static void SaveAllLocalData();

	void ResponseCallback(FHttpRequestPtr, FHttpResponsePtr, bool);
	void TestPrint();

	TArray<UAnalyticsData*> Buffer;
	TArray<UValueTracker*> Trackers;
	
	FString Ip = "";
	FString Port = "";
	bool bSecureHTTP = true;

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
	static UAnalyticsData* MakeAnalyticsData(EAnalyticTypeEnum Type);
	
	UFUNCTION(BlueprintPure, Category = "RiddlebitAnalytics")
	UAnalyticsData* AddField(FString Key, FString Data);

	UFUNCTION(BlueprintPure, Category = "RiddlebitAnalytics")
	UAnalyticsData* AddVectorField(FString Key, FVector Data);

	FString ToJsonString();

	UAnalyticsData();
	EAnalyticTypeEnum Type;

private:
	FString TimeStamp;

	TMap<FString, FString> Fields;
};
