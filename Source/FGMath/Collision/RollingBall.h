#pragma once

// Ported from other project

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RollingBall.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EBallState : uint8
{
	Grounded,
	Airborne,
};

UCLASS()
class FGMATH_API ARollingBall : public AActor
{
	GENERATED_BODY()

public:
	ARollingBall();

	//Collider Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereCollider;
	
	//Mesh Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball Mesh")
	TObjectPtr<UStaticMeshComponent> BallMesh;

public:
	//Ball Setup Variables
	/** Magnitude limit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Setup Variables", meta=(ClampMin="0"))
	float MaxVelocity;
	/** Min magnitude for a bounce to occur */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Setup Variables", meta=(ClampMin="0"))
	float MinBounceVelocity;
	/** Force we use to slow ball in air*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Setup Variables", meta=(ClampMin="0", ForceUnits="cm/s"))
	float AirDrag;
	/** Force we use to slow ball on ground */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Setup Variables", meta=(ClampMin="0", ForceUnits="cm/s"))
	float GroundedDrag;
	/** GravityScale */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball Setup Variables")
	float GravityScale;
	/** Scale for the slope force */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Setup Variables", meta=(ClampMin="0"))
	float SlopeGravityScale;
	/** Multiplier applied to velocity each bounce */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Setup Variables", meta=(ClampMin="0"))
	float VelocityReductionMultiplierPerBounce;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball Variables")
	EBallState CurrentBallState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball Variables")
	FVector CurrentVelocity;

protected:
	UPROPERTY(Transient)
	FVector ForceToAdd;
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category="Ball|Debug")
	FVector LastFloorNormal;
	UPROPERTY(Transient)
	float PhysicsTimer;

public:
	UFUNCTION(BlueprintCallable)
	void AddImpulse(FVector Force);

protected:
	UFUNCTION()
	void BallPhysics(float DeltaTime);

	UFUNCTION()
	void PhysGrounded(float DeltaTime);
	UFUNCTION()
	void PhysAirborne(float DeltaTime);

	UFUNCTION()
	void UpdateVelocity(float DeltaTime);
	UFUNCTION()
	void PerformMove(FVector MoveDelta, float DeltaTime);
	UFUNCTION()
	void GroundCheck();
	UFUNCTION()
	bool FloorSweep();
	UFUNCTION()
	void ApplyDrag(float DragForce, float DeltaTime);
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};