
// 편하게 개발하기 위한 전처리 헤더
#pragma once

#include "CoreMinimal.h"

// 생성한 프로필에 대하여 이름 명명
#define CPROFILE_OATCAPSULE TEXT("OatCapsule")
#define CPROFILE_OATTRIGGER TEXT("OatTrigger")
#define CPROFILE_BLOCKEXCEPTCAMERA TEXT("BlockExceptCamera")  // 카메라 빼고 전부 막음 (근데 얘 안씀;;)
#define CPROFILE_ENEMYCAPSULE TEXT("EnemyCapsule")  // OatAction에만 반응


// 생성한 트레이스 채널에 대하여 이름 명명
#define CCHANNEL_OATACTION ECC_GameTraceChannel1
#define CCHANNEL_ENEMYACTION ECC_GameTraceChannel2
