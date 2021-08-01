#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AnalyticsManager.h"

class FRiddlebitAnalyticsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
