#pragma once
#include "Math.hpp"

AMATH_NAMESPACE


struct Ray
{
	Vector3 origin;
	Vector3 direction;

	Ray(const Vector3& _origin, const Vector3& _direction) : origin(_origin), direction(_direction) {}

	Vector3 At(float t) const { return origin + (direction * t); }
};

struct HitRecord
{
	Vector3 point;
	Vector3 normal;
	float t;
	bool frontFace;

	inline void SetFaceNormal(const Ray& ray, const Vector3& outwardNormal)
	{
		frontFace = Vector3::Dot(ray.direction, outwardNormal);
		normal = frontFace ? outwardNormal : outwardNormal * -1;
	}
}; 

class Hittable
{
public:
	virtual bool Hit(const Ray& ray, float t_max, HitRecord& record) const = 0;
};

class Sphere final : Hittable
{
public:
	float radius;
	Vector3 center;
	Sphere() : radius(1), center(Vector3::Zero()) {}
	Sphere(float _radius, const Vector3& _center) : radius(_radius), center(_center) {}
	
	bool Hit(const Ray& ray, float t_max, HitRecord& record) const override
	{
		Vector3 oc	= ray.origin - center;
		float a		= ray.direction.LengthSquared();
		float half_b = Vector3::Dot(oc, ray.direction);
		float c		= oc.LengthSquared() - radius * radius;
		float discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;
		float sqrtd = sqrtf(discriminant);

		float root = (-half_b - sqrtd) / a;
		constexpr float t_min = 0.001;
		
		if (root < t_min || t_max)
		{
			root = (-half_b + sqrtd) / a;
			if (root < t_min || t_max < root) return false;
		}

		record.t = root;
		record.point = ray.At(record.t);
		record.normal = (record.point - center) / radius;
		return true;
	}
};

class Cube : Hittable
{
public:
	bool Hit(const Ray& ray, float t_max, HitRecord& record) const override
	{
	
	}
};

class Plane : Hittable
{
public:
	bool Hit(const Ray& ray, float t_max, HitRecord& record) const override
	{

	}
};

struct Camera
{

};


AMATH_END_NAMESPACE