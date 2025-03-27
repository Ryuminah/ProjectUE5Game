#pragma once

UENUM(BlueprintType)
enum class ELevelType: uint8
{
	LOBBY = 0,
	SELECT = 1,
	MAINSTAGE = 2,
	DUNGEON = 3,
};

UENUM()
enum class ECCEffectType : uint8
{
	NONE = 0, // CC¾øÀ½
	STUN = 1, //
	SLOW = 2,
};


UENUM(BlueprintType)
enum class EStageSectionState : uint8
{
	READYBATTLE = 0,
	INBATTLE = 1,
	ENDBATTLE = 2,
	NONE = 3,
};


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	IDLE = 0,
	RUN = 1,
	ATTACK = 2,
	HIT = 3,
	STUN = 4,
	DEAD = 5,
};
