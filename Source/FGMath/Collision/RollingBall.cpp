#include "RollingBall.h"

#include "Components/SphereComponent.h"
#include "FGMath/CustomMath.h"

#define SMALL_VELOCITY .1f
#define FLOOR_Z .75f
#define FLOOR_SLOPE_Z .95f
#define FLOOR_DIST_UPPER 5.0f
#define FLOOR_DIST 2.0f

ARollingBall::ARollingBall()
{
	MaxVelocity = 25000.f;
	AirDrag = 100.f;
	GravityScale = 900.f;
	GroundedDrag = 300.f;
	SlopeGravityScale = 1.5f;
	MinBounceVelocity = 1.f;
	VelocityReductionMultiplierPerBounce = .75f;
	
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->InitSphereRadius(50.f);
	SphereCollider->SetSimulatePhysics(false);
	SphereCollider->SetCollisionObjectType(ECC_PhysicsBody);

	SetRootComponent(SphereCollider);

	// Create ball mesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	BallMesh->SetupAttachment(SphereCollider);
	BallMesh->SetSimulatePhysics(false);
	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PrimaryActorTick.bCanEverTick = true;
}

void ARollingBall::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentBallState = EBallState::Airborne;
}

void ARollingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!CurrentVelocity.IsZero())
	{
		const float RotationalSpeed = CurrentVelocity.Length()/SphereCollider->GetScaledSphereRadius();
		const FVector RotationDirection = FVector::CrossProduct(CurrentVelocity.GetSafeNormal(), GetActorUpVector());

		BallMesh->AddWorldRotation(FCustomMath::VectorToRotator(RotationalSpeed * DeltaTime * RotationDirection));
	}
	
	PhysicsTimer += DeltaTime;

	//Only update at 60 frames
	if(PhysicsTimer > 1.f/60.f)
	{
		BallPhysics(PhysicsTimer);
		PhysicsTimer = 0;		
	}	
}

void ARollingBall::BallPhysics(float DeltaTime)
{
	switch (CurrentBallState)
	{
	case EBallState::Grounded:
		PhysGrounded(DeltaTime);
		break;
	case EBallState::Airborne:
		PhysAirborne(DeltaTime);
		break;
	}
}

void ARollingBall::PhysGrounded(float DeltaTime)
{
	UpdateVelocity(DeltaTime);

	//Apply gravity slope gravity
	const FVector Gravity = (FVector::DownVector * GravityScale*SlopeGravityScale);

	CurrentVelocity = FVector::VectorPlaneProject( CurrentVelocity + Gravity*DeltaTime, LastFloorNormal);

	PerformMove(CurrentVelocity*DeltaTime, DeltaTime);

	GroundCheck();
	
	ApplyDrag(GroundedDrag, DeltaTime);
}

void ARollingBall::PhysAirborne(float DeltaTime)
{
	UpdateVelocity(DeltaTime);

	//Apply gravity
	const FVector Gravity = (FVector::DownVector * GravityScale);

	CurrentVelocity += Gravity * DeltaTime;

	PerformMove(CurrentVelocity*DeltaTime, DeltaTime);

	GroundCheck();

	ApplyDrag(AirDrag, DeltaTime);
}

void ARollingBall::UpdateVelocity(float DeltaTime)
{
	CurrentVelocity += ForceToAdd;
	ForceToAdd = FVector::ZeroVector;
}

void ARollingBall::PerformMove(FVector MoveDelta, float DeltaTime)
{
	FHitResult Sweep(1.f);
	FCollisionQueryParams QueryParams(FName(TEXT("BallSweep")), true,  this);
	FCollisionResponseParams ResponseParams;

	SphereCollider->InitSweepCollisionParams(QueryParams, ResponseParams);
	
	//Move ball
	AddActorWorldOffset(MoveDelta, true, &Sweep);

	if(Sweep.bBlockingHit)
	{
		float Time = Sweep.Time;

		//Depenetrate ball
		if(Sweep.bStartPenetrating)
		{
			AddActorWorldOffset(Sweep.Normal * (Sweep.PenetrationDepth+.1f));			
		}

		//Cache original normal if slides
		const FVector Normal = Sweep.ImpactNormal;
		
		FVector SlideVector = FVector::VectorPlaneProject(MoveDelta, Normal) * (1.f - Time);
		if (FVector::DotProduct(SlideVector, MoveDelta) > 0.f)
		{
			FHitResult SecondSweep(1.f);
			//UE_LOG(LogTemp, Warning, TEXT("Sliding: %s"), *SlideVector.ToString());
			AddActorWorldOffset(SlideVector, true, &SecondSweep);

			if(SecondSweep.bBlockingHit)
			{
				Sweep = SecondSweep;
			}
		}		

		//Only bounce if going fast enough
		if(FVector::DotProduct(-(CurrentVelocity), Normal) > MinBounceVelocity + GravityScale*DeltaTime)
		{
			CurrentVelocity = FMath::GetReflectionVector(CurrentVelocity, Normal);
			
			CurrentVelocity *= VelocityReductionMultiplierPerBounce;
			AddActorWorldOffset(Normal * .1f);
		}
		else
		{
			//Zero velocity on the normal plane
			CurrentVelocity -= CurrentVelocity*Normal.GetAbs();
		}
	}
}

void ARollingBall::GroundCheck()
{
	if(FloorSweep())
	{
		CurrentBallState = EBallState::Grounded;
	}
	else
	{
		CurrentBallState = EBallState::Airborne;
	}
}

bool ARollingBall::FloorSweep()
{
	FHitResult FloorHit(1.f);
	FCollisionQueryParams QueryParams(FName(TEXT("BallSweep")), false,  this);
	FCollisionResponseParams ResponseParams;
	FCollisionShape CollisionShape = SphereCollider->GetCollisionShape();

	SphereCollider->InitSweepCollisionParams(QueryParams, ResponseParams);

	const FVector StartLoc = GetActorLocation() + FVector::UpVector*FLOOR_DIST_UPPER;
	const FVector EndLoc = GetActorLocation() + FVector::DownVector*FLOOR_DIST;

	//Only set floored if z is low enough and and is within floor steepness
	if(CurrentVelocity.Z <= SMALL_VELOCITY && GetWorld()->SweepSingleByChannel(FloorHit,  StartLoc, EndLoc, FQuat::Identity,
		ECC_PhysicsBody, CollisionShape, QueryParams, ResponseParams) && FloorHit.ImpactNormal.Z > FLOOR_Z)
	{		
		LastFloorNormal = FloorHit.Normal;

		return true;
	}
	else
	{
		LastFloorNormal = FVector::ZeroVector;		
		return false;
	}
}

void ARollingBall::ApplyDrag(const float DragForce, const float DeltaTime)
{
	FVector Velocity = CurrentVelocity;
	
	float VelocityLength = Velocity.Length();

	//Reduce magnitude by drag
	VelocityLength = FMath::Max(0,VelocityLength - DragForce*DeltaTime);

	Velocity = Velocity.GetSafeNormal() * FMath::Min(VelocityLength, MaxVelocity);
	
	CurrentVelocity = Velocity;
}

void ARollingBall::AddImpulse(const FVector Force)
{
	ForceToAdd += Force;
	CurrentBallState = EBallState::Airborne;
}

