#pragma once

#include "CoreMinimal.h"
#include "DTLocalize.generated.h"

USTRUCT(BlueprintType)
struct FDTLocalize
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Key;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NameKey;

};
