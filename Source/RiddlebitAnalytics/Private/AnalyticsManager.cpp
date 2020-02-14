#include "..\Public\AnalyticsManager.h"

FHttpModule* UAnalyticsManager::Http = NULL;
UAnalyticsManager* UAnalyticsManager::Instance = NULL;

void UAnalyticsManager::InitAnalytics()
{
	UE_LOG(LogTemp, Warning, TEXT("Makka"));
	Http = &FHttpModule::Get();
	Instance = NewObject<UAnalyticsManager>();
}

void UAnalyticsManager::RegisterAnalytics(UAnalyticsData* Data)
{
	Instance->Buffer.Add(Data);
}

void UAnalyticsManager::PushAnalytics()
{
	if (Http != NULL) {
		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->SetURL("http://92.62.46.171:3000/api/ui-events");
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetContentAsString("[ " + Instance->Buffer[0]->ToJsonString() + " ]");
		Request->ProcessRequest();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Please call InitAnalytics() before anything else."));
	}
}

UAnalyticsData* UAnalyticsData::MakeAnalyticsData() {
	return NewObject<UAnalyticsData>();
}

UAnalyticsData* UAnalyticsData::AddField(FString Key, FString Value)
{
	Fields.Add(Key, Value);
	UE_LOG(LogTemp, Warning, TEXT("Pakka %s %s"), *Key, *Value);
	return this;
}

FString UAnalyticsData::ToJsonString()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("timestamp", TimeStamp);

	UE_LOG(LogTemp, Warning, TEXT("hei %i"), Fields.Num());

	for (auto& Elem : Fields) {
		JsonObject->SetStringField(Elem.Key, Elem.Value);
		UE_LOG(LogTemp, Warning, TEXT("Kukki %s %s"), *Elem.Key, *Elem.Value);
	}
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	UE_LOG(LogTemp, Warning, TEXT("json %s"), *OutputString);
	return OutputString;
}

UAnalyticsData::UAnalyticsData()
{
	FString UnixTime = FString::FromInt(FDateTime::UtcNow().ToUnixTimestamp());
	FString MilliSeconds = FString::FromInt(FDateTime::UtcNow().GetMillisecond());
	FVector Vec;
	int ZeroesToPrepend = 3 - MilliSeconds.Len();
	for (int i = 0; i < ZeroesToPrepend; ++i)
		MilliSeconds = "0" + MilliSeconds;

	TimeStamp = UnixTime + MilliSeconds;
	UE_LOG(LogTemp, Warning, TEXT("Time = %s"), *TimeStamp);
}
