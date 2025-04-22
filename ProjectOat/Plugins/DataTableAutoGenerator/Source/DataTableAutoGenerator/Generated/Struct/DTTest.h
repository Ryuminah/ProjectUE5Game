#pragma once

#include "CoreMinimal.h"
#include "DTTest.generated.h"

USTRUCT(BlueprintType)
struct FDTTest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Lv;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Atk;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AtkRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoveSpeed;

};
