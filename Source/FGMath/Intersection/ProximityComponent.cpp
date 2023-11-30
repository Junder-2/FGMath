#include "ProximityComponent.h"

#include "ProximitySubsystem.h"

UProximityComponent::UProximityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Radius = 1000.f;
}

void UProximityComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto SubSystem = GetWorld()->GetSubsystem<UProximitySubsystem>();
	SubSystem->RegisterProximityComponent(this);
}

void UProximityComponent::AddIntersectingComponent(UProximityComponent* NewComponent)
{
	if(!IntersectingComponents.Contains(NewComponent))
		IntersectingComponents.Add(NewComponent);
}

void UProximityComponent::ClearIntersectingComponents()
{
	IntersectingComponents.Empty();
}

void UProximityComponent::DrawShape(const FColor Color)
{
	if(bDrawn) return;
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), Radius,16, Color);
	bDrawn = true;
}

void UProximityComponent::ResetDrawn()
{
	bDrawn = false;
}

bool UProximityComponent::GetIntersectingComponents(TArray<UProximityComponent*>& Out) const
{
	Out = IntersectingComponents;
	return !IntersectingComponents.IsEmpty();
}

UProximityComponent* UProximityComponent::GetClosestIntersection()
{
	UProximityComponent* ClosestComponent = nullptr;
	float ClosestDist = 100000;

	FVector MyLocation = GetOwner()->GetActorLocation();

	for (UProximityComponent* Element : IntersectingComponents)
	{
		const float Dist = FVector::DistXY(MyLocation, Element->GetOwner()->GetActorLocation()); 
		if(Dist < ClosestDist)
		{
			ClosestDist = Dist;
			ClosestComponent = Element;
		}
	}

	return ClosestComponent;
}
