#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProximityComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FGMATH_API UProximityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProximityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius;

protected:
	UPROPERTY()
	bool bDrawn;
	UPROPERTY()
	TArray<UProximityComponent*> IntersectingComponents;
	
public:
	void ResetDrawn();
	void AddIntersectingComponent(UProximityComponent* NewComponent);
	void ClearIntersectingComponents();
	
	UFUNCTION(BlueprintCallable)
	bool GetIntersectingComponents(TArray<UProximityComponent*>& Out) const;
	UFUNCTION(BlueprintCallable)
	UProximityComponent* GetClosestIntersection();
	
	void DrawShape(const FColor Color);
	
public:
	virtual void BeginPlay() override;
};
