#include "..\Public\AnalyticsManager.h"

FHttpModule* UAnalyticsManager::Http = NULL;
UAnalyticsManager* UAnalyticsManager::Instance = NULL;

void UAnalyticsManager::InitAnalytics()
{
	Http = &FHttpModule::Get();
	Instance = NewObject<UAnalyticsManager>();
}

void UAnalyticsManager::RegisterAnalytics(UAnalyticsData* Data)
{
	Instance->Buffer.Add(Data);
}

void UAnalyticsManager::PushAnalytics()
{
	if (Http != NULL && Instance->Buffer.Num() > 0) {
		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->SetURL("http://92.62.46.171:3000/api/ui-events");
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		FString Content = "[ ";
		for (int i = 0; i < Instance->Buffer.Num() - 1; ++i) {
			Content += Instance->Buffer[i]->ToJsonString() + ", ";
		}
		Content += Instance->Buffer[Instance->Buffer.Num() - 1]->ToJsonString() + " ]";
		Request->SetContentAsString(Content);
		Request->ProcessRequest();
		Instance->Buffer.Empty();
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
	return this;
}

FString UAnalyticsData::ToJsonString()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("timestamp", TimeStamp);

	for (auto& Elem : Fields) {
		JsonObject->SetStringField(Elem.Key, Elem.Value);
	}
	FString OutputString;

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
}

UAnalyticsData::UAnalyticsData()
{
	FString UnixTime = FString::FromInt(FDateTime::UtcNow().ToUnixTimestamp());
	FString MilliSeconds = FString::FromInt(FDateTime::UtcNow().GetMillisecond());

	int ZeroesToPrepend = 3 - MilliSeconds.Len();
	for (int i = 0; i < ZeroesToPrepend; ++i)
		MilliSeconds = "0" + MilliSeconds;

	TimeStamp = UnixTime + MilliSeconds;
}
