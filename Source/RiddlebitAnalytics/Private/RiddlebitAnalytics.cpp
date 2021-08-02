#include "RiddlebitAnalytics.h"

#define LOCTEXT_NAMESPACE "FRiddlebitAnalyticsModule"

void FRiddlebitAnalyticsModule::StartupModule()
{
	UAnalyticsManager::InitAnalytics(TEXT("127.0.0.1"), TEXT("8080"));
}

void FRiddlebitAnalyticsModule::ShutdownModule()
{
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRiddlebitAnalyticsModule, RiddlebitAnalytics)