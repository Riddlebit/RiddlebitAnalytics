// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RiddlebitAnalytics.h"

#define LOCTEXT_NAMESPACE "FRiddlebitAnalyticsModule"

void FRiddlebitAnalyticsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Warning, TEXT("RBAnal module startup, meme 3"));
	AnalyticsManager.InitAnalytics(TEXT("127.0.0.1"), TEXT("8080"));
}

void FRiddlebitAnalyticsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRiddlebitAnalyticsModule, RiddlebitAnalytics)