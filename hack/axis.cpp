#include <cmath>

#include "axis.h"

Axis::Axis()
{
	pitch = 0;
	yaw = 0;
}

Axis::Axis(float pitch, float yaw)
{
	this->pitch = pitch;
	this->yaw = yaw;
}

float Axis::Abs()
{
	return (float)sqrt(pitch * pitch + yaw * yaw);
}

Axis Axis::SubtractNorm(Axis b)
{
	Axis diff;
	if (yaw > 90.f && b.yaw < -90.f) diff.yaw = b.yaw - yaw + 360.f;
	else diff.yaw = yaw - b.yaw;

	diff.pitch = pitch - b.pitch;
	if (diff.pitch > 88.f) diff.pitch = 88.f;
	if (diff.pitch < -88.f) diff.pitch = -88.f;
	return diff;
}

Axis Axis::Subtract(Axis b)
{
	Axis diff;
	diff.yaw = yaw - b.yaw;
	diff.pitch = pitch - b.pitch;
	return diff;
}

Axis Axis::DistanceAbs(Axis b)
{
	Axis diff;

	diff.yaw = std::abs(yaw - b.yaw);
	if (diff.yaw > 180.f) diff.yaw = 360.f - diff.yaw;

	diff.pitch = std::abs(pitch - b.pitch);

	return diff;
}

bool Axis::ReferenceEntityRightsided(Axis b)
{
	float f = b.yaw - yaw;
	if (f < -180.f) f += 360.f;
	return f > 0;
}

bool Axis::ReferenceEntityHigher(Axis b)
{
	return pitch < b.pitch;
}

void Axis::SubtractDirect(Axis b)
{
	if (std::isnan(b.yaw) || std::isnan(b.pitch)) return;

	if (yaw > 90.f && b.yaw < -90.f) yaw = b.yaw - yaw + 360.f;
	else yaw -= b.yaw;

	if (pitch - b.pitch > 88.f) pitch = 88.f;
	else if (pitch - b.pitch < -88.f) pitch = -88.f;
	else pitch -= b.pitch;
}

Axis Axis::AddNorm(Axis b)
{
	Axis sum;
	sum.pitch = pitch + b.pitch;
	if (sum.pitch > 180.f) sum.pitch -= 360;
	if (sum.pitch < 180.f) sum.pitch += 360;

	sum.yaw = yaw + b.yaw;
	if (sum.yaw > 89.f) sum.yaw = 89.f;
	if (sum.yaw < -89.f) sum.yaw = -89.f;

	return sum;
}

Axis Axis::Add(Axis b)
{
	Axis sum;
	sum.yaw = yaw + b.yaw;
	sum.pitch = pitch + b.pitch;
	return sum;
}

void Axis::AddDirect(Axis b)
{
	yaw += b.yaw;
	pitch += b.pitch;
}

Axis Axis::Multiply(float f)
{
	return Axis(pitch * f, yaw * f);
}

void Axis::MultiplyDirect(float f)
{
	pitch *= f;
	yaw *= f;
}