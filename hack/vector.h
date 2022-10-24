#pragma once
#include "Axis.h"

const float PI = 3.141592654f;

struct Vector
{
	Vector();
	Vector(float x, float y, float z);

	float Abs();

	inline Vector operator-(const Vector& v) { return Vector(x - v.x, y - v.y, z - v.z); }
	inline Vector operator+(const Vector& v) { return Vector(x + v.x, y + v.y, z + v.z); }
	inline Vector operator*(const float f) { return Vector(x * f, y * f, z * f); }
	inline Vector operator-();
	inline Vector& operator+=(const Vector& v);

	static bool AngleDEG(float* angle, Vector a, Vector b);
	Vector UnitVector();
	static Axis AxisFromVector(Vector vector);

	float x;
	float y;
	float z;
};