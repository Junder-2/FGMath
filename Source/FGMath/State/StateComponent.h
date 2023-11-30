#pragma once

#include "CoreMinimal.h"
#include "StateConstants.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FGMATH_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStateComponent();

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite, Category="State|Debug", meta=(Bitmask, BitmaskEnum = ERelativeContext))
	int32 ActorState;
	
public:	
	virtual void BeginPlay() override;
};
