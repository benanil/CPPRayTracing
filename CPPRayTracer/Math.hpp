#pragma once
#include <immintrin.h>
#include <math.h>

using byte = unsigned char;

struct Vector2i
{
	union
	{
		struct { int x, y; };
		int arr[2];
	};

	Vector2i() : x(0), y(0) {}
	Vector2i(int scale) : x(scale), y(scale) {}
	Vector2i(int _x, int _y) : x(_x), y(_y) {}

	inline float Length()			{ return sqrt(LengthSquared()); }
	inline float LengthSquared()	{ return x * x + y * y; }

	void Normalize() { *this /= Length(); }

	inline Vector2i operator + (Vector2i b) const { return Vector2i(x + b.x, y + b.y); }
	inline Vector2i operator - (Vector2i b) const { return Vector2i(x - b.x, y - b.y); }
	inline Vector2i operator * (Vector2i b) const { return Vector2i(x * b.x, y * b.y); }
	inline Vector2i operator / (Vector2i b) const { return Vector2i(x / b.x, y / b.y); }

	inline Vector2i operator += (Vector2i b) { x += b.x; y += b.y; return *this; }
	inline Vector2i operator -= (Vector2i b) { x -= b.x; y -= b.y; return *this; }
	inline Vector2i operator *= (Vector2i b) { x *= b.x; y *= b.y; return *this; }
	inline Vector2i operator /= (Vector2i b) { x /= b.x; y /= b.y; return *this; }

	inline Vector2i operator *  (int b) const { return Vector2i(x * b, y * b); }
	inline Vector2i operator /  (int b) const { return Vector2i(x / b, y / b); }
	inline Vector2i operator *= (int b) { x *= b; y *= b; return *this; }
	inline Vector2i operator /= (int b) { x /= b; y /= b; return *this; }
};

struct Vector2
{
	union
	{
		struct { float x, y; };
		float arr[2];
	};
	
	Vector2() : x(0), y(0) {}
	Vector2(float scale) : x(scale), y(scale) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	inline float Length()			{ return sqrt(LengthSquared()); }
	inline float LengthSquared()	{ return x * x + y * y; }

	void Normalize() { *this /= Length(); }

	inline Vector2 operator + (Vector2 b) const { return Vector2(x + b.x, y + b.y); }
	inline Vector2 operator - (Vector2 b) const { return Vector2(x - b.x, y - b.y); }
	inline Vector2 operator * (Vector2 b) const { return Vector2(x * b.x, y * b.y); }
	inline Vector2 operator / (Vector2 b) const { return Vector2(x / b.x, y / b.y); }

	inline Vector2 operator += (Vector2 b) { x += b.x; y += b.y; return *this; }
	inline Vector2 operator -= (Vector2 b) { x -= b.x; y -= b.y; return *this; }
	inline Vector2 operator *= (Vector2 b) { x *= b.x; y *= b.y; return *this; }
	inline Vector2 operator /= (Vector2 b) { x /= b.x; y /= b.y; return *this; }

	inline Vector2 operator *  (float b) const { return Vector2(x * b, y * b); }
	inline Vector2 operator /  (float b) const { return Vector2(x / b, y / b); }
	inline Vector2 operator *= (float b) { x *= b; y *= b; return *this; }
	inline Vector2 operator /= (float b)		{ x /= b; y /= b; return *this; }
};

struct Vector3
{
	union
	{
		struct { float x, y, z; };
		float arr[3];
		__m128 vec;
	};

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float scale) : x(scale), y(scale), z(scale) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(__m128 _vec) : vec(_vec) {}

	inline float Length() const			{ return sqrt(LengthSquared()); }
	inline float LengthSquared() const	{ return x * x + y * y + z * z; }

	void Normalize() { *this /= Length(); }

	inline static float Dot(const Vector3& a, const Vector3& b)
	{
		return a.arr[0] * b.arr[0] + a.arr[1] * b.arr[1] + a.arr[2] * b.arr[2];
	}

	inline static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
	{
		return Vector3(
			a.x + (b.x - a.x) * t,
			a.y + (b.y - a.y) * t,
			a.z + (b.z - a.z) * t
		);
	}

	inline static Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.arr[1] * b.arr[2] - a.arr[2] * b.arr[1],
			           a.arr[2] * b.arr[0] - a.arr[0] * b.arr[2],
			           a.arr[0] * b.arr[1] - a.arr[1] * b.arr[0]);
	}

	inline static Vector3 Normalize(const Vector3& a)
	{
		return a / a.Length();
	}

	inline Vector3 operator + (const Vector3& b) const { return _mm_add_ps(vec, b.vec); }
	inline Vector3 operator - (const Vector3& b) const { return _mm_sub_ps(vec, b.vec); }
	inline Vector3 operator * (const Vector3& b) const { return _mm_mul_ps(vec, b.vec); }
	inline Vector3 operator / (const Vector3& b) const { return _mm_div_ps(vec, b.vec); }

	inline Vector3 operator += (const Vector3& b) { vec = _mm_add_ps(vec, b.vec); return *this; }
	inline Vector3 operator -= (const Vector3& b) { vec = _mm_sub_ps(vec, b.vec); return *this; }
	inline Vector3 operator *= (const Vector3& b) { vec = _mm_mul_ps(vec, b.vec); return *this; }
	inline Vector3 operator /= (const Vector3& b) { vec = _mm_div_ps(vec, b.vec); return *this; }

	inline Vector3 operator * (float b) const { return _mm_mul_ps(vec, _mm_set_ps1(b)); }
	inline Vector3 operator / (float b) const { return _mm_div_ps(vec, _mm_set_ps1(b)); }
	inline Vector3 operator *= (float b)	  { vec  = _mm_mul_ps(vec, _mm_set_ps1(b)); return *this; }
	inline Vector3 operator /= (float b)	  { vec  = _mm_div_ps(vec, _mm_set_ps1(b)); return *this; }
};

struct Color32
{
	union
	{	
		struct { byte r, g, b, a; };
		byte arr[4];
		int _int;
	};
	Color32() : r(0), g(0), b(0), a(0) {};
	Color32(__int32 __int) : _int(__int) {};
	Color32(byte _r, byte _g, byte _b) : r(_r), g(_g), b(_b), a(255) {};
	Color32(byte _r, byte _g, byte _b, byte _a) : r(_r), g(_g), b(_b), a(_a) {};
};

struct Color
{
	union
	{
		struct { float r, g, b, a; };
		float arr[4];
		__m128 vec;
	};

	static constexpr float OneDiv255 = 1.0f / 255.0f;

	Color() : r(0), g(0), b(0), a(0) {}
	Color(Color32 col) : r(col.r * OneDiv255), g(col.g * OneDiv255), b(col.b * OneDiv255), a(col.a * OneDiv255) {}
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1) {}
	Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	Color(__m128 _vec) : vec(_vec) {}

	inline float Length()			{ return sqrt(LengthSquared()); }
	inline float LengthSquared()	{ return r * r + g * g + b * b; }

	void Normalize() { *this /= Length(); }

	inline static float Dot(const Color& a, const Color& b)
	{
		return a.arr[0] * b.arr[0] + a.arr[1] * b.arr[1] + a.arr[2] * b.arr[2];
	}
	
	// lerp
	inline static Color Mix(const Color& a, const Color& b, float t)
	{
		return Color(
			a.r + (b.r - a.r) * t,
			a.g + (b.g - a.g) * t,
			a.b + (b.b - a.b) * t
		);
	}

	Color32 ConvertToColor32() {
		Color converted = *this * 255.0f;
		return Color32(byte(converted.r), byte(converted.g), byte(converted.b));
	}

	inline Color operator + (const Color& b) const { return _mm_add_ps(vec, b.vec); }
	inline Color operator - (const Color& b) const { return _mm_sub_ps(vec, b.vec); }
	inline Color operator * (const Color& b) const { return _mm_mul_ps(vec, b.vec); }
	inline Color operator / (const Color& b) const { return _mm_div_ps(vec, b.vec); }

	inline Color operator += (const Color& b) { vec = _mm_add_ps(vec, b.vec); return *this; }
	inline Color operator -= (const Color& b) { vec = _mm_sub_ps(vec, b.vec); return *this; }
	inline Color operator *= (const Color& b) { vec = _mm_mul_ps(vec, b.vec); return *this; }
	inline Color operator /= (const Color& b) { vec = _mm_div_ps(vec, b.vec); return *this; }

	inline Color operator * (float b) const { return _mm_mul_ps(vec, _mm_set_ps1(b)); }
	inline Color operator / (float b) const { return _mm_div_ps(vec, _mm_set_ps1(b)); }
	inline Color operator *= (float b)	    { vec  = _mm_mul_ps(vec, _mm_set_ps1(b)); return *this; }
	inline Color operator /= (float b)		{ vec  = _mm_div_ps(vec, _mm_set_ps1(b)); return *this; }
};

struct Vector4
{
	union
	{
		struct { float x, y, z, w; };
		float arr[4];
		__m128 vec;
	};

	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(float scale) : x(scale), y(scale), z(scale), w(scale) {}
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4(__m128 _vec) : vec(_vec) {}

	inline Vector4 operator + (const Vector4& b) const { return _mm_add_ps(vec, b.vec); }
	inline Vector4 operator - (const Vector4& b) const { return _mm_sub_ps(vec, b.vec); }
	inline Vector4 operator * (const Vector4& b) const { return _mm_mul_ps(vec, b.vec); }
	inline Vector4 operator / (const Vector4& b) const { return _mm_div_ps(vec, b.vec); }

	inline Vector4 operator += (const Vector4& b) { vec = _mm_add_ps(vec, b.vec); return *this; }
	inline Vector4 operator -= (const Vector4& b) { vec = _mm_sub_ps(vec, b.vec); return *this; }
	inline Vector4 operator *= (const Vector4& b) { vec = _mm_mul_ps(vec, b.vec); return *this; }
	inline Vector4 operator /= (const Vector4& b) { vec = _mm_div_ps(vec, b.vec); return *this; }
};
