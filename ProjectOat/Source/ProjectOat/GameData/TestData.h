
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TestData.generated.h"

USTRUCT(BlueprintType)
struct FTestData :  public FTableRowBase
{
	GENERATED_BODY()

public:
	FTestData() : Lv(0.f), MaxHP(0.f), Atk(0.f), AtkRange(0.f), MoveSpeed(0.f){}

	UPROPERTY(Category = Stat, EditAnywhere, BlueprintReadWrite)
	float Lv;

	UPROPERTY(Category = Stat, EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(Category = Stat, EditAnywhere, BlueprintReadWrite)
	float Atk;

	UPROPERTY(Category = Stat, EditAnywhere, BlueprintReadWrite)
	float AtkRange;

	UPROPERTY(Category = Stat, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;


	FTestData operator+(const FTestData& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FTestData Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FTestData) / sizeof(float);

		// 전부 float이라고 가정하고 계산
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}

};