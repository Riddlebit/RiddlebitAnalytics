#include "..\Public\AnalyticsManager.h"

FHttpModule* UAnalyticsManager::Http = NULL;
UAnalyticsManager* UAnalyticsManager::Instance = NULL;

void UAnalyticsManager::InitAnalytics(FString Ip, FString Port)
{
	Http = &FHttpModule::Get();
	Instance = NewObject<UAnalyticsManager>();
	Instance->Ip = Ip;
	Instance->Port = Port;
	Instance->bSecureHTTP = false;
	Instance->AddToRoot(); // To prevent the singleton from being garbage collected
	
	ULevel* Level = NULL;
	UWorld* World = NULL;

	//Instance->GetWorld()->OnLevelsChanged().AddUObject(Instance, &UAnalyticsManager::TestPrint);
	//->BindUObject(Instance, &UAnalyticsManager::TestPrint);
}

void UAnalyticsManager::RegisterFloatTracker(float& FloatPointer)
{
	UValueTracker* Tracker = NewObject<UValueTracker>();
	Tracker->AddToRoot(); // We will destroy this ourselves.
	Tracker->Value = &FloatPointer;
	Instance->Trackers.Add(Tracker);
	Instance->GetWorld()->GetTimerManager().SetTimer(Tracker->TimerHandle, Tracker, &UValueTracker::PrintValue, 5.0f, true);
}

void UAnalyticsManager::TestPrint() 
{
	UE_LOG(LogTemp, Warning, TEXT("Delegate Test"));
}

void UAnalyticsManager::RegisterAnalytics(UAnalyticsData* Data)
{
	Instance->Buffer.Add(Data);
}

void UAnalyticsManager::PushAnalytics()
{
	if (Http != NULL) {	
		if (Instance->Buffer.Num() > 0) {
			TMap<EAnalyticTypeEnum, TArray<UAnalyticsData*>> DataTypeMap;
			for (int i = 0; i < Instance->Buffer.Num(); ++i) {
				if (DataTypeMap.Contains(Instance->Buffer[i]->Type)) {
					DataTypeMap[Instance->Buffer[i]->Type].Add(Instance->Buffer[i]);
				}
				else {
					DataTypeMap.Add(Instance->Buffer[i]->Type);
					DataTypeMap[Instance->Buffer[i]->Type].Add(Instance->Buffer[i]);
				}
			}

			for (auto& Elem : DataTypeMap) {
				TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

				Request->OnProcessRequestComplete().BindUObject(Instance, &UAnalyticsManager::ResponseCallback);
				FString Protocol = Instance->bSecureHTTP ? "https" : "http";
				FString Url = Protocol + "://" + Instance->Ip + ":" + Instance->Port + "/";
				Url += UAnalyticTypes::GetApiUrl(Elem.Key);
				Request->SetURL(Url);
				Request->SetVerb("POST");
				Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
				Request->SetHeader("Content-Type", TEXT("application/json"));
				FString Content = "[ ";
				for (int i = 0; i < Elem.Value.Num() - 1; ++i) {
					Content += Elem.Value[i]->ToJsonString() + ", ";
				}
				Content += Elem.Value[Elem.Value.Num() - 1]->ToJsonString() + " ]";
				Request->SetContentAsString(Content);
				Request->ProcessRequest();
			}
			Instance->Buffer.Empty();
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Please call InitAnalytics() before anything else."));
	}
}

void UAnalyticsManager::ResponseCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{

	}
}

UAnalyticsData* UAnalyticsData::MakeAnalyticsData(EAnalyticTypeEnum Type) {
	UAnalyticsData* AnalyticsData = NewObject<UAnalyticsData>();
	AnalyticsData->Type = Type;
	AnalyticsData->AddToRoot();
	return  AnalyticsData;
}

UAnalyticsData* UAnalyticsData::AddField(FString Key, FString Value)
{
	Fields.Add(Key, Value);
	return this;
}

UAnalyticsData* UAnalyticsData::AddVectorField(FString Key, FVector Data)
{
	FString VecStr = FString::SanitizeFloat(Data.X) + "," + FString::SanitizeFloat(Data.Y) + "," + FString::SanitizeFloat(Data.Z);
	return AddField(Key, VecStr);
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
