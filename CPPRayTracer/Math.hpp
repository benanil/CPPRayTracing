#pragma once
#include <immintrin.h>
#include <math.h>

using byte = unsigned char;

#define AMATH_NAMESPACE namespace AMath {
#define AMATH_END_NAMESPACE }

AMATH_NAMESPACE

// constants
constexpr float PI = 3.141592653;
constexpr float RadToDeg = 180.0f / PI;
constexpr float DegToRad = PI / 180.0f;

template<typename T>
inline float Max(T a, T b)	noexcept { return a > b ? a : b; }
template<typename T>
inline float Min(T a, T b)	noexcept { return a < b ? a : b; }
template<typename T>
inline float Clamp(T x, T a, T b)	noexcept { return Max(a, Min(b, x)); }
inline float IsZero(float x)		noexcept { return fabs(x) > 1e-10; }

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

	inline float Length()			{ return sqrtf(LengthSquared()); }
	inline float LengthSquared()	{ return x * x + y * y; }

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

	inline float Length()			{ return sqrtf(LengthSquared()); }
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

	inline Vector2 operator *  (float b) const	{ return Vector2(x * b, y * b); }
	inline Vector2 operator /  (float b) const  { return Vector2(x / b, y / b); }
	inline Vector2 operator *= (float b)		{ x *= b; y *= b; return *this; }
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

	inline float Length() const			{ return sqrtf(LengthSquared()); }
	inline float LengthSquared() const	{ return x * x + y * y + z * z; }

	void Normalize() { *this /= Length(); }

	inline static float Length(const Vector3& vec) { return vec.Length(); }

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

	inline Vector3 One()	{ return  Vector3(1, 1, 1); }
	inline Vector3 Zero()	{ return  Vector3(0, 0, 0); }
	inline Vector3 Up()		{ return  Vector3(0, 1, 0) ; } 
	inline Vector3 Right()	{ return  Vector3(1, 0, 0) ; } 
	inline Vector3 Forward(){ return  Vector3(0, 0, 1) ; } 
	inline Vector3 Left()	{ return  Vector3(-1, 0, 0); } 
	inline Vector3 Back()	{ return  Vector3(0, 0, -1); } 
	inline Vector3 Down()	{ return  Vector3(0, -1, 0); } 

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

	inline static __m128 Dot(const Vector4& V1, const Vector4& V2)
	{
		__m128 vTemp2 = V2.vec;
		__m128 vTemp = _mm_mul_ps(V1.vec, vTemp2);
		vTemp2	= _mm_shuffle_ps(vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0)); // Copy X to the Z position and Y to the W position
		vTemp2	= _mm_add_ps(vTemp2, vTemp);          // Add Z = X+Z; W = Y+W;
		vTemp	= _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0));  // Copy W to the Z position
		vTemp	= _mm_add_ps(vTemp, vTemp2);           // Add Z and W together
		return _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(2, 2, 2, 2));    // Splat Z and return
	}

	inline static __m128 Normalize(const __m128 V)
	{
		__m128 vLengthSq = _mm_mul_ps(V, V);
		__m128 vTemp	= _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
		vLengthSq		= _mm_add_ps(vLengthSq, vTemp);
		vLengthSq		= _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
		vTemp			= _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
		vLengthSq		= _mm_add_ps(vLengthSq, vTemp);
		vLengthSq		= _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 vResult  = _mm_sqrt_ps(vLengthSq);
		__m128 vZeroMask= _mm_setzero_ps();
		vZeroMask		= _mm_cmpneq_ps(vZeroMask, vResult);
		vLengthSq		= _mm_cmpneq_ps(vLengthSq, _mm_set_ps(0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000));
		vResult			= _mm_div_ps(V, vResult);
		vResult			= _mm_and_ps(vResult, vZeroMask);
		__m128 vTemp1	= _mm_andnot_ps(vLengthSq, _mm_set_ps(0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000));
		__m128 vTemp2	= _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);
		return vResult;
	}

	inline Vector4 operator + (const Vector4& b) const { return _mm_add_ps(vec, b.vec); }
	inline Vector4 operator - (const Vector4& b) const { return _mm_sub_ps(vec, b.vec); }
	inline Vector4 operator * (const Vector4& b) const { return _mm_mul_ps(vec, b.vec); }
	inline Vector4 operator / (const Vector4& b) const { return _mm_div_ps(vec, b.vec); }

	inline Vector4 operator += (const Vector4& b) { vec = _mm_add_ps(vec, b.vec); return *this; }
	inline Vector4 operator -= (const Vector4& b) { vec = _mm_sub_ps(vec, b.vec); return *this; }
	inline Vector4 operator *= (const Vector4& b) { vec = _mm_mul_ps(vec, b.vec); return *this; }
	inline Vector4 operator /= (const Vector4& b) { vec = _mm_div_ps(vec, b.vec); return *this; }

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

	static __forceinline Color32 Red()		{ return Color32(255, 0, 0, 255);	}
	static __forceinline Color32 Green()	{ return Color32(0, 255, 0, 255);	}
	static __forceinline Color32 Blue()		{ return Color32(0, 0, 255, 255);	}
	static __forceinline Color32 Orange()	{ return Color32(128, 128, 0, 255); }
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

	inline static Color MixWithA(const Color& a, const Color& b, float t)
	{
		Color result = Mix(a, b, t);
		result.a = a.a + (b.a - a.a) * t;
		return result;
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

struct Matrix4
{
	union
	{
		__m128   r[4];
		Vector4  vec[4];
		struct  { __m128  r1, r2, r3, r4; };
		struct { Vector4 vec1, vec2, vec3, vec4; };
		float m[4][4];
	};

	Matrix4() {}
	Matrix4(const __m128& x, const __m128& y, const __m128& z, const __m128& w) : r1(x), r2(y), r3(z), r4(w) {}
	Matrix4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w) : vec1(x), vec2(y), vec3(z), vec4(w) {}

	// THIS MATRIX CODES COPIED FROM DIRECTX MATH

	inline static Matrix4 Identity()
	{
		return Matrix4(
			_mm_set_ps(1, 0, 0, 0),
			_mm_set_ps(0, 1, 0, 0),
			_mm_set_ps(0, 0, 1, 0),
			_mm_set_ps(0, 0, 0, 1)
		);
	}

	inline static Matrix4 XMMatrixOrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
	{
		Matrix4 M;
		float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
		float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
		float fRange = 1.0f / (FarZ - NearZ);
		__m128 rMem = {
			fReciprocalWidth,
			fReciprocalHeight,
			fRange,
			1.0f
		};
		__m128  rMem2 = {
			-(ViewLeft + ViewRight),
			-(ViewTop + ViewBottom),
			-NearZ,
			1.0f
		};
		__m128 vValues = rMem;
		__m128 vTemp = _mm_setzero_ps();
		vTemp = _mm_move_ss(vTemp, vValues);
		vTemp = _mm_add_ss(vTemp, vTemp);
		M.r[0] = vTemp;
		vTemp = vValues;
		vTemp = _mm_and_ps(vTemp, _mm_set_ps(0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000));
		vTemp = _mm_add_ps(vTemp, vTemp);
		M.r[1] = vTemp;
		vTemp = vValues;
		vTemp = _mm_and_ps(vTemp, _mm_set_ps(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000));
		M.r[2] = vTemp;
		vValues = _mm_mul_ps(vValues, rMem2);
		M.r[3] = vValues;
		return M;
	}

	inline static Matrix4 PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
	{
		Matrix4 M;
		float CosFov = cos(FovAngleY * 0.5);
		float SinFov = sin(FovAngleY * 0.5); 
		float fRange = FarZ / (FarZ - NearZ);
		// Note: This is recorded on the stack
		float Height = CosFov / SinFov;
		__m128 rMem = _mm_set_ps(
			Height / AspectRatio,
			Height,
			fRange,
			-fRange * NearZ
		);
		__m128 vValues = rMem;
		__m128 vTemp = _mm_setzero_ps();
		vTemp = _mm_move_ss(vTemp, vValues);
		M.r[0] = vTemp;
		vTemp = vValues;
		vTemp = _mm_and_ps(vTemp, _mm_set_ps(0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000));
		M.r[1] = vTemp;
		vTemp = _mm_setzero_ps();
		vValues = _mm_shuffle_ps(vValues, _mm_set_ps(0,0,0,1), _MM_SHUFFLE(3, 2, 3, 2));
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
		M.r[2] = vTemp;
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
		M.r[3] = vTemp;
		return M;
	}

	inline static Matrix4 Transpose(const Matrix4& M)
	{
		const __m128 vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
		const __m128 vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
		const __m128 vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
		const __m128 vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
		return Matrix4(
			_mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0)),
			_mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1)),
			_mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0)),
			_mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1))
		);
	}

	// from directx math:  xnamathmatrix.inl
	inline Matrix4 XMMatrixInverse(const Matrix4& M1, const Matrix4& M2)
	{
		Matrix4 MT = Matrix4::Transpose(M1);
		__m128 V00 = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
		__m128 V10 = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
		__m128 V01 = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
		__m128 V11 = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
		__m128 V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
		__m128 V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));

		__m128 D0 = _mm_mul_ps(V00, V10);
		__m128 D1 = _mm_mul_ps(V01, V11);
		__m128 D2 = _mm_mul_ps(V02, V12);

		V00 = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
		V10 = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
		V01 = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
		V11 = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
		V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
		V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		D0 = _mm_sub_ps(D0, V00);
		D1 = _mm_sub_ps(D1, V01);
		D2 = _mm_sub_ps(D2, V02);
		// V11 = D0Y,D0W,D2Y,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
		V00 = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
		V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
		V01 = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
		V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
		// V13 = D1Y,D1W,D2W,D2W
		__m128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
		V02 = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
		V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
		__m128 V03 = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
		V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

		__m128 C0 = _mm_mul_ps(V00, V10);
		__m128 C2 = _mm_mul_ps(V01, V11);
		__m128 C4 = _mm_mul_ps(V02, V12);
		__m128 C6 = _mm_mul_ps(V03, V13);

		// V11 = D0X,D0Y,D2X,D2X
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
		V00 = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
		V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V01 = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
		V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
		// V13 = D1X,D1Y,D2Z,D2Z
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
		V02 = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
		V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
		V03 = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
		V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		C0 = _mm_sub_ps(C0, V00);
		C2 = _mm_sub_ps(C2, V01);
		C4 = _mm_sub_ps(C4, V02);
		C6 = _mm_sub_ps(C6, V03);

		V00 = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
		V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
		V10 = _mm_shuffle_ps(V10, V10, _MM_SHUFFLE(0, 2, 3, 0));
		V01 = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
		V11 = _mm_shuffle_ps(V11, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V02 = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
		V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
		V12 = _mm_shuffle_ps(V12, V12, _MM_SHUFFLE(0, 2, 3, 0));
		V03 = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
		V13 = _mm_shuffle_ps(V13, V13, _MM_SHUFFLE(2, 1, 0, 3));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		__m128 C1 = _mm_sub_ps(C0, V00);
		C0 = _mm_add_ps(C0, V00);
		__m128 C3 = _mm_add_ps(C2, V01);
		C2 = _mm_sub_ps(C2, V01);
		__m128 C5 = _mm_sub_ps(C4, V02);
		C4 = _mm_add_ps(C4, V02);
		__m128 C7 = _mm_add_ps(C6, V03);
		C6 = _mm_sub_ps(C6, V03);

		C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
		C0 = _mm_shuffle_ps(C0, C0, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C2, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C4, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C6, _MM_SHUFFLE(3, 1, 2, 0));
		// Get the determinate
		__m128 vTemp = Vector4::Dot(C0, MT.r[0]);
		vTemp = _mm_div_ps(_mm_set_ps1(1), vTemp);
		return Matrix4(
			_mm_mul_ps(C0, vTemp),
			_mm_mul_ps(C2, vTemp),
			_mm_mul_ps(C4, vTemp),
			_mm_mul_ps(C6, vTemp)
		);
	}

	inline Matrix4 XMMatrixMultiply(Matrix4 M1, Matrix4 M2 )
	{
		Matrix4 mResult;
		// Use vW to hold the original row
		__m128 vW = M1.r[0];
		// Splat the component X,Y,Z then W
		__m128 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		// Perform the opertion on the first row
		vX = _mm_mul_ps(vX, M2.r[0]);
		vY = _mm_mul_ps(vY, M2.r[1]);
		vZ = _mm_mul_ps(vZ, M2.r[2]);
		vW = _mm_mul_ps(vW, M2.r[3]);
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[0] = vX;
		// Repeat for the other 3 rows
		vW = M1.r[1];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M2.r[0]);
		vY = _mm_mul_ps(vY, M2.r[1]);
		vZ = _mm_mul_ps(vZ, M2.r[2]);
		vW = _mm_mul_ps(vW, M2.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[1] = vX;
		vW = M1.r[2];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M2.r[0]);
		vY = _mm_mul_ps(vY, M2.r[1]);
		vZ = _mm_mul_ps(vZ, M2.r[2]);
		vW = _mm_mul_ps(vW, M2.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[2] = vX;
		vW = M1.r[3];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M2.r[0]);
		vY = _mm_mul_ps(vY, M2.r[1]);
		vZ = _mm_mul_ps(vZ, M2.r[2]);
		vW = _mm_mul_ps(vW, M2.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[3] = vX;
		return mResult;
	}
};

inline Vector4 Vector3Transform(const Vector3& V, const Matrix4 M)
{
	__m128 vResult = _mm_shuffle_ps(V.vec, V.vec, _MM_SHUFFLE(0, 0, 0, 0));
	vResult = _mm_mul_ps(vResult, M.r[0]);
	__m128 vTemp = _mm_shuffle_ps(V.vec, V.vec, _MM_SHUFFLE(1, 1, 1, 1));
	vTemp	= _mm_mul_ps(vTemp, M.r[1]);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp	= _mm_shuffle_ps(V.vec, V.vec, _MM_SHUFFLE(2, 2, 2, 2));
	vTemp	= _mm_mul_ps(vTemp, M.r[2]);
	vResult = _mm_add_ps(vResult, vTemp);
	vResult = _mm_add_ps(vResult, M.r[3]);
	return vResult;
}

struct Quaternion
{
	union
	{
		struct { float x, y, z, w; };
		float arr[4];
		__m128 vec;
	};

	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(float scale) : x(scale), y(scale), z(scale), w(scale) {}
	Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	Quaternion(__m128 _vec) : vec(_vec) {}

	static __m128 Mul (const __m128& Q1, const __m128& Q2) 
	{
		static const __m128 ControlWZYX = { 1.0f,-1.0f, 1.0f,-1.0f };
		static const __m128 ControlZWXY = { 1.0f, 1.0f,-1.0f,-1.0f };
		static const __m128 ControlYXWZ = { -1.0f, 1.0f, 1.0f,-1.0f };
		__m128 Q2X = Q2, Q2Y = Q2, Q2Z = Q2, vResult = Q2;
		vResult = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(3, 3, 3, 3));
		Q2X = _mm_shuffle_ps(Q2X, Q2X, _MM_SHUFFLE(0, 0, 0, 0));
		Q2Y = _mm_shuffle_ps(Q2Y, Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
		Q2Z = _mm_shuffle_ps(Q2Z, Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
		vResult = _mm_mul_ps(vResult, Q1);
		__m128 Q1Shuffle = Q1;
		Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
		Q2X = _mm_mul_ps(Q2X, Q1Shuffle);
		Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
		Q2X = _mm_mul_ps(Q2X, ControlWZYX);
		Q2Y = _mm_mul_ps(Q2Y, Q1Shuffle);
		Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
		Q2Y = _mm_mul_ps(Q2Y, ControlZWXY);
		Q2Z = _mm_mul_ps(Q2Z, Q1Shuffle);
		vResult = _mm_add_ps(vResult, Q2X);
		Q2Z = _mm_mul_ps(Q2Z, ControlYXWZ);
		Q2Y = _mm_add_ps(Q2Y, Q2Z);
		vResult = _mm_add_ps(vResult, Q2Y);
		return vResult;
	}

	inline static __m128 Dot(const Quaternion& V1, const Quaternion& V2)
	{
		__m128 vTemp2 = V2.vec;
		__m128 vTemp = _mm_mul_ps(V1.vec, vTemp2);
		vTemp2 = _mm_shuffle_ps(vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0)); // Copy X to the Z position and Y to the W position
		vTemp2 = _mm_add_ps(vTemp2, vTemp);          // Add Z = X+Z; W = Y+W;
		vTemp = _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0));  // Copy W to the Z position
		vTemp = _mm_add_ps(vTemp, vTemp2);           // Add Z and W together
		return _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(2, 2, 2, 2));    // Splat Z and return
	}

	static inline Quaternion Slerp(const Quaternion& Q0, const Quaternion& Q1, float t)
	{
		// Result = Q0 * sin((1.0 - t) * Omega) / sin(Omega) + Q1 * sin(t * Omega) / sin(Omega)
		const __m128 T = _mm_set_ps1(t);
		__m128 Omega, CosOmega, SinOmega, V01, S0, S1, Sign, Result;
		static const __m128 Zero = _mm_set_ps1(0);
		static const __m128 One = _mm_set_ps1(1);
		static const __m128 NegativeOne = _mm_set_ps1(-1);

		static const __m128 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };
		static const __m128 SignMask2 = { 0x80000000,0x00000000,0x00000000,0x00000000 };
		static const __m128 MaskXY = { 0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000 };

		CosOmega = Quaternion::Dot(Q0, Q1);

		__m128  Control = _mm_cmplt_ps(CosOmega, Zero);

		__m128  selectTemp1 = _mm_andnot_ps(Control, One);
		__m128  selectTemp2 = _mm_and_ps(NegativeOne, Control);

		Sign	= _mm_or_ps(selectTemp1, selectTemp2);
		CosOmega = _mm_mul_ps(CosOmega, Sign);
		Control = _mm_cmplt_ps(CosOmega, OneMinusEpsilon);
		
		SinOmega = _mm_mul_ps(CosOmega, CosOmega);
		SinOmega = _mm_sub_ps(One, SinOmega);
		SinOmega = _mm_sqrt_ps(SinOmega);

		Omega = _mm_atan2_ps(SinOmega, CosOmega);

		V01 = _mm_shuffle_ps(T, T, _MM_SHUFFLE(2, 3, 0, 1));
		V01 = _mm_and_ps(V01, MaskXY);
		V01 = _mm_xor_ps(V01, SignMask2);
		V01 = _mm_add_ps(_mm_set_ps(1,0,0,0), V01);

		S0 = _mm_mul_ps(V01, Omega);
		S0 = _mm_sin_ps(S0);
		S0 = _mm_div_ps(S0, SinOmega);

		selectTemp1 = _mm_andnot_ps(Control, V01);
		selectTemp2 = _mm_and_ps(S0, Control);

		S0 = _mm_or_ps(selectTemp1, selectTemp2);

		S1 = _mm_shuffle_ps(S0, S0, _MM_SHUFFLE(1, 1, 1, 1));
		S0 = _mm_shuffle_ps(S0, S0, _MM_SHUFFLE(0, 0, 0, 0));

		S1 = _mm_mul_ps(S1, Sign);
		Result = _mm_mul_ps(Q0.vec, S0);
		S1 = _mm_mul_ps(S1, Q1.vec);
		Result = _mm_add_ps(Result, S1);
		return Result;
	}

	inline Quaternion operator *  (const Quaternion& b)	{  return Mul(this->vec, b.vec); }
	inline Quaternion operator *= (const Quaternion& b) { this->vec = Mul(this->vec, b.vec); return *this; }
};

inline Quaternion EulerToQuaternion(Vector3 euler) noexcept
{
	// Abbreviations for the various angular functions
	euler.x *= 0.5f; euler.y *= 0.5f; euler.z *= 0.5f;

	const float c1 = cos(euler.x);  const float s1 = sin(euler.x);
	const float c2 = cos(euler.y);  const float s2 = sin(euler.y);
	const float c3 = cos(euler.z);  const float s3 = sin(euler.z);
	return Quaternion(
		(s1 * c2 * c3) + (c1 * s2 * s3),
		(c1 * s2 * c3) - (s1 * c2 * s3),
		(c1 * c2 * s3) + (s1 * s2 * c3),
		(c1 * c2 * c3) - (s1 * s2 * s3)
	);
}

inline Vector3 QuatToEulerAngles(const Quaternion& q) noexcept {
	Vector3 eulerAngles;

	// Threshold for the singularities found at the north/south poles.
	constexpr float SINGULARITY_THRESHOLD = 0.4999995f;

	const float sqw = q.w * q.w;
	const float sqx = q.x * q.x;
	const float sqy = q.y * q.y;
	const float sqz = q.z * q.z;
	const float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	const float singularityTest = (q.x * q.z) + (q.w * q.y);

	if (singularityTest > SINGULARITY_THRESHOLD * unit) {
		eulerAngles.z = 2.0f * atan2(q.x, q.w);
		eulerAngles.y = PI / 2;
		eulerAngles.x = 0;
	}
	else if (singularityTest < -SINGULARITY_THRESHOLD * unit)
	{
		eulerAngles.z = -2.0f * atan2(q.x, q.w);
		eulerAngles.y = -(PI / 2);
		eulerAngles.x = 0;
	}
	else {
		eulerAngles.z = atan2(2 * ((q.w * q.z) - (q.x * q.y)), sqw + sqx - sqy - sqz);
		eulerAngles.y = asin(2 * singularityTest / unit);
		eulerAngles.x = atan2(2 * ((q.w * q.x) - (q.y * q.z)), sqw - sqx - sqy + sqz);
	}
	return eulerAngles;
}

inline float Repeat(const float& t, const float& length) noexcept
{
	return Clamp(t - floorf(t / length) * length, 0.0f, length);
}

inline float LerpAngle(const float& a, const float& b, const float& t) noexcept
{
	float delta = Repeat((b - a), 360);
	if (delta > 180) delta -= 360;
	return a + delta * Clamp(t, 0.0f, 1.0f);
}

inline Vector3 ExtractPosition(const Matrix4& matrix) noexcept
{
	Vector4 row3 = matrix.r[3];
	return Vector3(row3.y, row3.z, row3.w);
}

inline Vector3 ExtractScale(const Matrix4& matrix) noexcept
{
	return Vector3(Vector3::Length(Vector3(matrix.r[0])),
				   Vector3::Length(Vector3(matrix.r[2])),
				   Vector3::Length(Vector3(matrix.r[1])));
}

inline Quaternion ExtractRotation(const Matrix4 matrix, bool rowNormalize = true) noexcept
{
	Vector4 row0 = matrix.vec[0];
	Vector4 row1 = matrix.vec[1];
	Vector4 row2 = matrix.vec[2];

	if (rowNormalize) {
		row0 = Vector3::Normalize(row0.vec).vec;
		row1 = Vector3::Normalize(row1.vec).vec;
		row2 = Vector3::Normalize(row2.vec).vec;
	}

	// code below adapted from Blender
	Quaternion q;
	const float trace = 0.25 * (row0.x + row1.y + row2.z + 1.0);

	if (trace > 0) {
		const float sq = sqrtf(trace);
		const float oneDivSq = 1.0 / (4.0 * sq);

		q = Quaternion((row1.z - row2.y) * oneDivSq,
			(row2.x - row0.z) * oneDivSq,
			(row0.y - row1.x) * oneDivSq,
			sq);
	}
	else if (row0.x > row1.y && row0.x > row2.z)
	{
		float sq = 2.0 * sqrtf(1.0 + row0.x - row1.y - row2.z);
		const float oneDivSq = 1.0 / sq;

		q = Quaternion(0.25 * sq,
			(row1.x + row0.y) * oneDivSq,
			(row2.x + row0.z) * oneDivSq,
			(row2.y - row1.z) * oneDivSq);
	}
	else if (row1.y > row2.z)
	{
		float sq = 2.0 * sqrtf(1.0f + row1.y - row0.x - row2.z);
		const float oneDivSq = 1.0 / sq;

		q = Quaternion((row1.x + row0.y) * oneDivSq,
			0.25 * sq,
			(row2.y + row1.z) * oneDivSq,
			(row2.x - row0.z) * oneDivSq);
	}
	else {
		float sq = 2.0 * sqrtf(1.0f + row2.z - row0.x - row1.y);
		const float oneDivSq = 1.0 / sq;

		q = Quaternion((row2.x + row0.z) * oneDivSq,
			(row2.y + row1.z) * oneDivSq,
			0.25 * sq,
			(row1.x - row0.y) * oneDivSq);
	}
	q = Vector4::Normalize(q.vec);
	return q;
}

inline Vector3  RadianToDegree(const Vector3& radian) {
	return Vector3(radian.x * RadToDeg, radian.y * RadToDeg, radian.z * RadToDeg);
}

inline Vector3  DegreeToRadian(const Vector3& degree) {
	return Vector3(degree.x * DegToRad, degree.y * DegToRad, degree.z * DegToRad);
}

inline void RadianToDegree(const Vector3& radian, Vector3& degree) {
	degree.x = radian.x * RadToDeg;
	degree.y = radian.y * RadToDeg;
	degree.z = radian.z * RadToDeg;
}

inline void DegreeToRadian(const Vector3& degree, Vector3& radian) {
	radian.x = degree.x * DegToRad;
	radian.y = degree.y * DegToRad;
	radian.z = degree.z * DegToRad;
}


AMATH_END_NAMESPACE