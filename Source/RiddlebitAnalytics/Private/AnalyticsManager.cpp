#include "..\Public\AnalyticsManager.h"

FHttpModule* UAnalyticsManager::Http = NULL;
UAnalyticsManager* UAnalyticsManager::Instance = NULL;

void UAnalyticsManager::InitAnalytics(FString Ip, FString Port)
{
	Http = &FHttpModule::Get();
	Instance = NewObject<UAnalyticsManager>();
	Instance->Ip = Ip;
	Instance->Port = Port;
	Instance->AddToRoot();
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
				TSharedRef<IHttpRequest> Request = Http->CreateRequest();

				Request->OnProcessRequestComplete().BindUObject(Instance, &UAnalyticsManager::ResponseCallback);
				
				FString Url = "http://" + Instance->Ip + ":" + Instance->Port + "/";
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
	UE_LOG(LogTemp, Warning, TEXT("Hopsi"));
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

void UAnalyticsManager::ResponseTestCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Hopsi"));
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		auto Arr = JsonObject->GetArrayField("playerEvents");
		for (int i = 0; i < Arr.Num(); ++i) {
			auto Obj = Arr[i]->AsObject();
		
			FString VecStr = Obj->GetStringField("position");
			TArray<FString> ArrStr;
			VecStr.ParseIntoArray(ArrStr, TEXT(","), true);
			FVector Vec;
			Vec.X = FCString::Atof(*ArrStr[0]);
			Vec.Y = FCString::Atof(*ArrStr[1]);
			Vec.Z = FCString::Atof(*ArrStr[2]);
			Instance->Positions.Add(Vec);
	
		
			FString RotVecStr = Obj->GetStringField("rotation");
			TArray<FString> RotArrStr;
			RotVecStr.ParseIntoArray(RotArrStr, TEXT(","), true);
			FVector RotVec;
			RotVec.X = FCString::Atof(*RotArrStr[0]);
			RotVec.Y = FCString::Atof(*RotArrStr[1]);
			RotVec.Z = FCString::Atof(*RotArrStr[2]);
			Instance->Rotations.Add(RotVec);
		}
	}
}
void UAnalyticsManager::GetShoots()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(Instance, &UAnalyticsManager::ResponseTestCallback);
	//This is the url on which to process the request
	Request->SetURL("http://92.62.46.171:3000/api/events/player");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

TArray<FVector> UAnalyticsManager::GetPositions()
{
	return Instance->Positions;
}

TArray<FVector> UAnalyticsManager::GetRotations()
{
	return Instance->Rotations;
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
