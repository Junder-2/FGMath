#include "ProximitySubsystem.h"

#include "IntersectionHelpers.h"
#include "ProximityComponent.h"

void UProximitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UProximitySubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for(const auto Prox : ProximityComponents)
	{
		Prox->ResetDrawn();
		Prox->ClearIntersectingComponents(); //Instead of clearing a onEnter, onExit design might be better
	}

	if(ProximityComponents.Num() == 1)
	{
		ProximityComponents[0]->DrawShape(FColor::Green);
		return;
	}	

	for(const auto ProxA : ProximityComponents)
	{
		for(const auto ProxB : ProximityComponents)
		{
			if(ProxA == ProxB)
				continue;

			FVector ContactPoint = FVector();

			if(UIntersectionHelpers::SphereSphere(
				ProxA->GetOwner()->GetActorLocation(), ProxA->Radius,
				ProxB->GetOwner()->GetActorLocation(), ProxB->Radius,
				ContactPoint
			))
			{
				DrawDebugPoint(GetWorld(), ContactPoint, 25.f, FColor::Cyan);

				ProxA->DrawShape(FColor::Red);
				ProxA->AddIntersectingComponent(ProxB);
				ProxB->DrawShape(FColor::Red);
				ProxB->AddIntersectingComponent(ProxA);
			}
		}
	}
}

void UProximitySubsystem::RegisterProximityComponent(UProximityComponent* ProximityComponent)
{
	if(!ProximityComponents.Contains(ProximityComponent))
		ProximityComponents.Add(ProximityComponent);
}

void UProximitySubsystem::UnregisterProximityComponent(UProximityComponent* ProximityComponent)
{
	if(ProximityComponents.Contains(ProximityComponent))
		ProximityComponents.Remove(ProximityComponent);
}

TStatId UProximitySubsystem::GetStatId() const
{
	return GetStatID();
}
