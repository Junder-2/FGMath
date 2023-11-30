#pragma once

UENUM(BlueprintType, meta=(BitFlags))
enum class ERelativeContext : uint8
{
	None = 0 UMETA(Hidden),
	InRange = 1,
	Front = 2,
	Right = 3,
	Above = 4,
};

UENUM(BlueprintType, meta=(BitFlags))
enum class EActorState : uint8
{
	None = 0 UMETA(Hidden),
	Crouched = 1,
	Dazed = 2,
};
