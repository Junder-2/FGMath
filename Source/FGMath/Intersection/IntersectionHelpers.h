#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IntersectionHelpers.generated.h"

UCLASS()
class FGMATH_API UIntersectionHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Sphere-Sphere Intersection */
	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static bool SphereSphere(const FVector CenterA, const float RadiusA, const FVector CenterB, const float RadiusB, FVector& ContactPoint)
	{
		const FVector Diff = CenterA - CenterB;
		const float RadiusSum = RadiusA + RadiusB;

		const auto bIntersects = Diff.Dot(Diff) <= RadiusSum * RadiusSum;

		if(bIntersects)
		{
			const auto Direction = CenterB - CenterA;
			RaySphere(CenterA, Direction.GetSafeNormal(), CenterB, RadiusB, ContactPoint);
		}

		return bIntersects;
	}

	/** Line-Sphere Intersection */
	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static bool RaySphere(const FVector Origin, const FVector Direction, const FVector Center, const float Radius, FVector& ContactPoint)
	{
		const FVector W = Center - Origin;
		const float WSQ = W.Dot(W);
		const float Projection = W.Dot(Direction);
		const float RSQ = Radius * Radius;

		// If sphere is behind ray, no intersection
		if(Projection < 0.f && WSQ > RSQ)
			return false;

		const float VSQ = Direction.Dot(Direction);

		// Diff vs radius
		const bool bIntersects = (VSQ * WSQ - Projection * Projection <= VSQ * Radius * Radius);

		if(bIntersects)
		{
			const float B = 2.f * Projection;
			const float C = WSQ - Radius * Radius;
			const float Discriminant = B * B - 4 * VSQ * C;
			const float T = (B - FMath::Sqrt(Discriminant)) / (2.f * VSQ);

			if(Discriminant < 0)
				ContactPoint = Origin + (Direction * -1.f);
			else
				ContactPoint = Origin + (Direction * T);
		}
		
		return bIntersects;
	}
};
