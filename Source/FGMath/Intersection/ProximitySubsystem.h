#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ProximitySubsystem.generated.h"

class UProximityComponent;

UCLASS()
class FGMATH_API UProximitySubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UProximityComponent*> ProximityComponents;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	
	void RegisterProximityComponent(UProximityComponent* ProximityComponent);
	void UnregisterProximityComponent(UProximityComponent* ProximityComponent);

	virtual TStatId GetStatId() const override;
};
