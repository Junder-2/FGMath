#pragma once

struct FCustomMath
{
	UE_NODISCARD static FRotator VectorToRotator(const FVector& Vector)
	{
		FRotator R;
		
		// Find yaw.
		R.Yaw = FMath::RadiansToDegrees(Vector.Z);
		// Find pitch.
		R.Pitch = FMath::RadiansToDegrees(Vector.Y);		
		// Find roll.
		R.Roll = FMath::RadiansToDegrees(Vector.X);
		
		return R;
	}
};
