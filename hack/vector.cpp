#include <cmath>

#include "vector.h"

Vector::Vector()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector::Abs()
{
	return (float)sqrt(x * x + y * y + z * z);
}

Vector Vector::operator-()
{
	return Vector(-x, -y, -z);
}

Vector& Vector::operator+=(const Vector& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

bool Vector::AngleDEG(float* angle, Vector a, Vector b)
{
	float a_abs_b_abs = a.Abs() * b.Abs();
	if (a_abs_b_abs == 0.f) return false;
	else
	{
		*angle = std::abs(180.f / PI * std::acos((a.x * b.x + a.y * b.y + a.z * b.z) / a_abs_b_abs));
		if (*angle > 90.f) *angle = 180 - *angle;
		return true;
	}
}

Vector Vector::UnitVector()
{
	float abs = this->Abs();
	if (abs == 0.f) return Vector();
	return Vector(x / abs, y / abs, z / abs);
}

Axis Vector::AxisFromVector(Vector vector)
{
	Axis axis;
	axis.pitch = (float)std::atan(vector.z / (float)sqrt(vector.x * vector.x + vector.y * vector.y)) * 180.f / PI;
	axis.yaw = (float)std::atan(vector.y / vector.x) / PI * 180.f;
	if (axis.yaw > 90.f) axis.yaw = 90.f;
	if (axis.yaw < -90.f) axis.yaw = -90.f;
	if (axis.pitch > 89.f) axis.pitch = 89.f;
	if (axis.pitch < -89.f) axis.pitch = -89.f;
	if (vector.x > 0 && vector.y > 0) axis.yaw -= 180.f;
	else if (vector.x > 0) axis.yaw += 180.f;
	return axis;
}