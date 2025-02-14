
#pragma once

UENUM(BlueprintType)
enum class  ELevelType: uint8
{
	LOBBY = 0,
	SELECT = 1,
	MAINSTAGE = 2,
	DUNGEON = 3,
};

UENUM()
enum class  ECCEffectType : uint8
{
	NONE = 0,				// CC¾øÀ½
	STUN = 1,				//
	SLOW = 2,
	
};
