#pragma once

class Axis
{
public:
	Axis();
	Axis(float pitch, float yaw);

	float Abs();
	Axis SubtractNorm(Axis b);
	Axis Subtract(Axis b);
	Axis DistanceAbs(Axis b);
	bool ReferenceEntityRightsided(Axis b);
	bool ReferenceEntityHigher(Axis b);
	void SubtractDirect(Axis b);
	Axis AddNorm(Axis b);
	Axis Add(Axis b);
	void AddDirect(Axis b);
	Axis Multiply(float f);
	void MultiplyDirect(float f);
	float pitch;
	float yaw;
};