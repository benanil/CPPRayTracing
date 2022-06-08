#include "RayTracer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include "External/stb_image.h"
#include "External/stb_image_write.h"
#include "Math.hpp"
#include "Structures.hpp"

using namespace AMath;

void RayTracer::Initialize()
{

}

bool HitSphere(const Vector3& center, double radius, const Ray& ray) {
    Vector3 oc = ray.origin - center;
    float a = Vector3::Dot(ray.direction, ray.direction);
    float b = 2.0 * Vector3::Dot(oc, ray.direction);
    float c = Vector3::Dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

Color RayColor(const Ray& ray)
{
    if (HitSphere(Vector3(0,0,-1), 0.5, ray)) {
        return Color(1.0, 0, 0);
    }

	const Vector3 unitDirection = Vector3::Normalize(ray.direction);
	const float t = 0.5f * (unitDirection.y + 1.0);
	const float oneMinusT = 1.0 - t;
	return Color(oneMinusT) + Color(0.5f, 0.7f, 1.0f) * t;
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
            image[j * image_width + i] = RayColor(r).ConvertToColor32();
        }
    }
	stbi_write_jpg("export.jpg", image_width, image_height, 4, image, 100);
    free(image);
}
