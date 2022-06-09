#include "RayTracer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include "External/stb_image.h"
#include "External/stb_image_write.h"
#include "Math.hpp"
#include "Structures.hpp"
#include <memory>
#include <vector>
#include <random>

#define MaxDepth 500

using namespace AMath;

inline double RandomFloat() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double RandomFloat(float min, float max) {
    return  min + (max - min) * RandomFloat();
}

inline static Vector3 RandomVec3()
{
    return Vector3(RandomFloat(), RandomFloat(), RandomFloat());
}

inline static Vector3 RandomVec3(float min, float max)
{
    return Vector3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
}

static inline Vector3 random_in_unit_sphere() {
    while (true) {
        auto p = RandomVec3(-1, 1);
        if (p.LengthSquared() >= 1) continue;
        return p;
    }
}

namespace RayTracer
{
    std::vector<Sphere> Spheres;
    
    float HitSphere(const Sphere& sphere, const Ray& ray, HitRecord& record);
    bool TraceSpheres(const Ray& ray, float t_max, HitRecord& record);
    Color RayColor(const Ray& ray, int depth);
}

bool RayTracer::TraceSpheres(const Ray& ray, float t_max, HitRecord& record) 
{
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (const auto& object : Spheres) {
        if (object.Hit(ray, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            record = temp_rec;
        }
    }

    return hit_anything;
}

void RayTracer::Initialize()
{
    Spheres.push_back(std::move(Sphere(0.5f, Vector3(0,0,-1))));
    Spheres.push_back(std::move(Sphere(100.0f, Vector3(0, -100.5, -1))));
}

float RayTracer::HitSphere(const Sphere& sphere, const Ray& ray, HitRecord& record)
{
    Vector3 oc = ray.origin - sphere.center;
    float a = Vector3::Dot(ray.direction, ray.direction);
    float b = 2.0 * Vector3::Dot(oc, ray.direction);
    float c = Vector3::Dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant <= 0) return -1.0f;
    else return (-b - sqrtf(discriminant)) / (2.0 * a);
}

Color RayTracer::RayColor(const Ray& ray, int depth)
{
    HitRecord record;
    float t;
    if (depth < 0) return Color(0);

    for (auto& sphere : Spheres)
    {
        t = HitSphere(sphere, ray, record);
        if (t > 0.0)
        {
            Vector3 N = Vector3::Normalize(ray.At(t) - Vector3(0, 0, -1));
            Vector3 target = record.point + record.normal + random_in_unit_sphere();
            return RayColor(Ray(record.point, target - record.point), depth-1) * 0.5f;
        }
    }

	const Vector3 unitDirection = Vector3::Normalize(ray.direction);
	t = 0.5f * (unitDirection.y + 1.0);
	const float oneMinusT = 1.0 - t;
	return Color(oneMinusT) + Color(0.5f * t, 0.7f * t, 1.0f * t) ;
}

void RayTracer::RenderFrame()
{
    // Image
    const float aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = int(image_width / aspect_ratio);

    // Camera

    const float viewport_height = 2.0;
    const float viewport_width = aspect_ratio * viewport_height;
    const float focal_length = 1.0;

    const Vector3 origin     = Vector3(0, 0, 0);
    const Vector3 horizontal = Vector3(viewport_width, 0, 0);
    const Vector3 vertical   = Vector3(0, viewport_height, 0);
    const Vector3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);

    Color32* image = (Color32*)malloc(image_width * image_height * sizeof(Color));

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            auto u = float(i) / (image_width - 1);
            auto v = float(j) / (image_height - 1);
            Ray r = Ray(origin, lower_left_corner + (horizontal * u) + (vertical * v) - origin);
            image[((image_height - j) * image_width) + i] = RayColor(r, MaxDepth).ConvertToColor32();
        }
    }
	stbi_write_jpg("export.jpg", image_width, image_height, 4, image, 900);
    free(image);
}
