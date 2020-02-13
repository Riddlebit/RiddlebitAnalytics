#include "..\Public\AnalyticsManager.h"

FHttpModule* UAnalyticsManager::Http = NULL;

void UAnalyticsManager::InitAnalytics()
{
	UE_LOG(LogTemp, Warning, TEXT("Makka"));
	Http = &FHttpModule::Get();
}

void UAnalyticsManager::PushAnalytics()
{
	if (Http != NULL) {
		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		Request->SetURL("http://92.62.46.171:3000/api/ui-events");
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->SetContentAsString("{ \"id\" : \"havi\" }");
		Request->ProcessRequest();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Please call InitAnalytics() before anything else."));
	}
}
