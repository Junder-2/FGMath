#pragma once

// Check if a mask has a single flag
#define HAS_BIT(Bitmask, Bit) (((Bitmask) & (1 << (int32)(Bit))) > 0)

#define HAS_ANY_BITS(Bitmask, Bits) (((Bitmask) & Bits) != 0)

// Check if a mask contains another mask
#define HAS_ALL_BITS(Bitmask, Bits) (((Bitmask) & Bits) == Bits)

// Set a specific bit
#define SET_BIT(Bitmask, Bit) (Bitmask |= 1 << (int32)(Bit))

// Clear a specific bit
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~(1 << (int32)(Bit)))

#include "CoreMinimal.h"
#include "StateConstants.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ContextHelpers.generated.h"

UCLASS()
class FGMATH_API UContextHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Context", meta = ( DefaultToSelf = Target ))
	static int32 GetRelativeContext(const AActor* Target, const AActor* Other, const float DistanceRange = 50.f)
	{
		int32 ReturnContext = 0;

		const FVector TargetLocation = Target->GetActorLocation();
		const FVector OtherLocation = Other->GetActorLocation();
		const FVector Direction = OtherLocation - TargetLocation;

		const FVector LocalSpaceLocation = Target->GetActorTransform().InverseTransformPosition(OtherLocation);

		if(LocalSpaceLocation.X > 0)
			SET_BIT(ReturnContext, ERelativeContext::Front);

		if(LocalSpaceLocation.Y > 0)
			SET_BIT(ReturnContext, ERelativeContext::Right);
		
		if(LocalSpaceLocation.Z > 0)
			SET_BIT(ReturnContext, ERelativeContext::Above);

		if(Direction.Length() < DistanceRange)
			SET_BIT(ReturnContext, ERelativeContext::InRange);
		
		return ReturnContext;
	}

	UFUNCTION(BlueprintCallable, Category = "Context")
	static FString GetRelativeContextString(const int32 RelativeContext)
	{
		FString ContextText;
		ContextText += HasFlag(RelativeContext, (int32)ERelativeContext::InRange) ? "InRange" : "OutsideRange";
		ContextText += HasFlag(RelativeContext, (int32)ERelativeContext::Front) ? "| Front" : "| Behind";
		ContextText += HasFlag(RelativeContext, (int32)ERelativeContext::Right) ? "| Right" : "| Left";
		ContextText += HasFlag(RelativeContext, (int32)ERelativeContext::Above) ? "| Above" : "| Underneath";
		return ContextText;
	}

	UFUNCTION(BlueprintCallable, Category = "Context")
	static FString GetActorStateString(const int32 RelativeContext)
	{
		FString StateText;
		StateText += HasFlag(RelativeContext, (int32)EActorState::Crouched) ? "Crouched" : "Standing";
		StateText += HasFlag(RelativeContext, (int32)EActorState::Dazed) ? "| Dazed" : "";
		return StateText;
	}

	UFUNCTION(BlueprintCallable, Category = "Context")
	static int32 SetFlag(int32 Bitmask, const int32 Value)
	{
		return SET_BIT(Bitmask, Value);
	}

	UFUNCTION(BlueprintCallable, Category = "Context")
	static int32 ClearFlag(int32 Bitmask, const int32 Value)
	{
		return CLEAR_BIT(Bitmask, Value);
	}
	
	UFUNCTION(BlueprintCallable, Category = "Context")
	static bool HasFlag(const int32 Test, const int32 Value)
	{
		return HAS_BIT(Test, Value);
	}

	UFUNCTION(BlueprintCallable, Category = "Context")
	static bool HasAllFlags(const int32 Test, const int32 Values)
	{
		return HAS_ALL_BITS(Test, Values);
	}	

	UFUNCTION(BlueprintCallable, Category = "Context")
	static bool HasAnyFlags(const int32 Test, const int32 Values)
	{
		return HAS_ANY_BITS(Test, Values);
	}
};
