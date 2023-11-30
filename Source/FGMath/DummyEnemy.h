#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyEnemy.generated.h"

class UProximityComponent;
class UCapsuleComponent;
class UStateComponent;

UCLASS()
class FGMATH_API ADummyEnemy : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category="State", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleCollider;

	UPROPERTY(Category="State", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStateComponent> StateComponent;

	UPROPERTY(Category="State", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UProximityComponent> ProximityComponent;

protected:
	UPROPERTY(Transient)
	bool bWantsCrouch;
	UPROPERTY(Transient)
	float CrouchProgress;
	UPROPERTY(Transient)
	float DefaultHeight;

	UPROPERTY(Transient)
	float RandomTimer;

public:
	ADummyEnemy();

	UFUNCTION(BlueprintCallable)
	UStateComponent* GetStateComponent() const;

	UFUNCTION(BlueprintCallable)
	UProximityComponent* GetProximityComponent() const;

	
protected:
	void SetWantsCrouch(bool bState);
	void CrouchLoop(float DeltaTime);
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
