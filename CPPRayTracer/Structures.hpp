#pragma once
#include "Math.hpp"

namespace AMath
{
	struct Sphere
	{
		float radius;
		Vector3 center;
		Sphere(float _radius, const Vector3& _center) : radius(_radius), center(_center) {}
	};

	struct Ray
	{
		Vector3 origin;
		Vector3 direction;

		Ray(const Vector3& _origin, const Vector3& _direction) : origin(_origin), direction(_direction) {}

		Vector3 At(float t) { return origin + (direction * t); }
	};

	struct Camera
	{

	};

	struct HitRecord
	{

	};
}