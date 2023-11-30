#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InterpolationHelpers.generated.h"

UCLASS()
class FGMATH_API UInterpolationHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:	
	/**  A Ease in Square with some cos and sine waves to add shakiness */
	UFUNCTION(BlueprintCallable, Category = "Interp")
	static float ShakyEase(const float Value)
	{
		return (Value*Value) - (FMath::Sin(Value*PI*2.f) * .1f) * FMath::Max(0, FMath::Cos(Value*PI*2.5f));
	}
	
};
