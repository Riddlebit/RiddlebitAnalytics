#pragma once

#include "CoreMinimal.h"
#include "AnalyticTypes.generated.h"

UENUM(BlueprintType)
enum class EAnalyticTypeEnum : uint8
{
    ATE_Player 	        UMETA(DisplayName = "Player"),
    ATE_Game            UMETA(DisplayName = "Game"),
    ATE_UserInterface   UMETA(DisplayName = "UI")
};

UCLASS()
class RIDDLEBITANALYTICS_API UAnalyticTypes : public UObject
{
    GENERATED_BODY()

public:
    static FString GetApiUrl(EAnalyticTypeEnum Type) {
        switch(Type){
        case EAnalyticTypeEnum::ATE_Player:
            return "/api/events/player";
        case EAnalyticTypeEnum::ATE_Game:
            return "/api/events/game";
        case EAnalyticTypeEnum::ATE_UserInterface:
            return "/api/events/ui";
        default:
            UE_LOG(LogTemp, Warning, TEXT("Invalid type in GetApiUrl: %i"), Type);
            return "";
        }
    }
};