#include "RiddlebitAnalytics.h"

#define LOCTEXT_NAMESPACE "FRiddlebitAnalyticsModule"

void FRiddlebitAnalyticsModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("RA Startup!"));
	UAnalyticsManager::InitAnalytics(TEXT("127.0.0.1"), TEXT("8080"));
}

void FRiddlebitAnalyticsModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("RA Shutdown!"));
	UAnalyticsManager::SaveAllLocalData();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRiddlebitAnalyticsModule, RiddlebitAnalytics)