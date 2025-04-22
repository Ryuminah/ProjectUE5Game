#pragma once

#include "CoreMinimal.h"
#include "DTEnemy.generated.h"

USTRUCT(BlueprintType)
struct FDTEnemy
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LocalizeKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Atk;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AtkRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DetectRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoveSpeed;

};
