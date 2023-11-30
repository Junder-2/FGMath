#include "DummyEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Interpolation/InterpolationHelpers.h"
#include "Intersection/ProximityComponent.h"
#include "State/ContextHelpers.h"
#include "State/StateComponent.h"

ADummyEnemy::ADummyEnemy()
{
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	CapsuleCollider->SetupAttachment(RootComponent);
	
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
	ProximityComponent = CreateDefaultSubobject<UProximityComponent>(TEXT("ProximityComponent"));
	
	PrimaryActorTick.bCanEverTick = true;
}	

void ADummyEnemy::BeginPlay()
{
	DefaultHeight = CapsuleCollider->GetUnscaledCapsuleHalfHeight();
	CrouchProgress = 0.f;
	Super::BeginPlay();
}

void ADummyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(RandomTimer <= 0)
	{
		RandomTimer = FMath::RandRange(.5f, 5.f);
		SetWantsCrouch(!bWantsCrouch);
	}
	else
	{
		RandomTimer -= DeltaTime;
	}

	CrouchLoop(DeltaTime);
}

void ADummyEnemy::CrouchLoop(float DeltaTime)
{
	if((bWantsCrouch && FMath::IsNearlyEqual(CrouchProgress, 1.f)) || (!bWantsCrouch && FMath::IsNearlyEqual(CrouchProgress, 0.f))) return;

	CrouchProgress = FMath::Clamp(CrouchProgress + (bWantsCrouch ? DeltaTime : -DeltaTime), 0.f, 1.f);

	const float OldHeight = CapsuleCollider->GetUnscaledCapsuleHalfHeight();
	const float InterpHeight = FMath::Lerp(DefaultHeight, DefaultHeight*.5f, UInterpolationHelpers::ShakyEase(CrouchProgress));

	const float HeightAdjust = (OldHeight - InterpHeight) * CapsuleCollider->GetShapeScale();
	
	CapsuleCollider->SetCapsuleHalfHeight(InterpHeight);
	CapsuleCollider->MoveComponent(FVector::DownVector*HeightAdjust, CapsuleCollider->GetComponentQuat(), true, nullptr, MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);

	if(CrouchProgress >= 0.5f)
	{		
		StateComponent->ActorState = UContextHelpers::SetFlag(StateComponent->ActorState, (int32)EActorState::Crouched);
	}
	else
	{
		StateComponent->ActorState = UContextHelpers::ClearFlag(StateComponent->ActorState, (int32)EActorState::Crouched);
	}
}

void ADummyEnemy::SetWantsCrouch(const bool bState)
{
	bWantsCrouch = bState;
}

UStateComponent* ADummyEnemy::GetStateComponent() const
{
	return StateComponent;
}

UProximityComponent* ADummyEnemy::GetProximityComponent() const
{
	return ProximityComponent;
}

