#pragma once
#include "Math.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

AMATH_NAMESPACE

struct Matrix4
{
	union
	{
		__m128   r[4];
		Vector4  vec[4];
		struct { __m128  r1, r2, r3, r4; };
		struct { Vector4 vec1, vec2, vec3, vec4; };
		float m[4][4];
	};

	Matrix4() : r1(_mm_set_ps(1, 0, 0, 0)), r2(_mm_set_ps(0, 1, 0, 0)), r3(_mm_set_ps(0, 0, 1, 0)), r4(_mm_set_ps(0, 0, 0, 1)) {}
	VECTORCALL Matrix4(const __m128& x, const __m128& y, const __m128& z, const __m128& w) : r1(x), r2(y), r3(z), r4(w) {}
	VECTORCALL Matrix4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w) : vec1(x), vec2(y), vec3(z), vec4(w) {}

	Matrix4  operator * (const Matrix4& b) { return Multiply(*this, b); }
	Matrix4& operator *= (const Matrix4& b) { *this = Multiply(*this, b); return *this; }

	// THIS MATRIX CODES COPIED FROM DIRECTX MATH

	FINLINE static Matrix4 Identity()
	{
		return Matrix4(
			_mm_set_ps(1, 0, 0, 0),
			_mm_set_ps(0, 1, 0, 0),
			_mm_set_ps(0, 0, 1, 0),
			_mm_set_ps(0, 0, 0, 1)
		);
	}

	static Matrix4 FromPosition(const float x, const float y, const float z)
	{
		Matrix4 result;
		result.r[0] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
		result.r[1] = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
		result.r[2] = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
		result.r[3] = _mm_set_ps(x, y, z, 1.0f);
		return result;
	}

	FINLINE static Matrix4 FromPosition(const Vector3& vec3)
	{
		return Matrix4(
			_mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f),
			_mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f),
			_mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f),
			_mm_set_ps(vec3.x, vec3.y, vec3.z, 1.0f)
		);
	}

	FINLINE static Matrix4 VECTORCALL LookAtLH(const Vector3 EyePosition, const Vector3 EyeDirection, const Vector3 UpDirection)
	{
		Vector3 R2 = EyeDirection;
		Vector3 R0 = Vector3::Cross(UpDirection, R2);
		R0 = Vector3::Normalize(R0);
		Vector3 R1 = Vector3::Cross(R2, R0);
		Vector3 NegEyePosition = _mm_mul_ps(EyePosition.vec, _mm_set_ps1(-1));
		
		Vector3 D0 = Vector3::Dot(R0, NegEyePosition);
		Vector3 D1 = Vector3::Dot(R1, NegEyePosition);
		Vector3 D2 = Vector3::Dot(R2, NegEyePosition);
		
		R0 = _mm_and_ps(R0.vec, _mm_set_ps(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000));
		R1 = _mm_and_ps(R1.vec, _mm_set_ps(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000));
		R2 = _mm_and_ps(R2.vec, _mm_set_ps(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000));
		
		D0 = _mm_and_ps(D0.vec, _mm_set_ps(0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF));
		D1 = _mm_and_ps(D1.vec, _mm_set_ps(0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF));
		D2 = _mm_and_ps(D2.vec, _mm_set_ps(0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF));
		
		D0 = _mm_or_ps(D0.vec, R0.vec);
		D1 = _mm_or_ps(D1.vec, R1.vec);
		D2 = _mm_or_ps(D2.vec, R2.vec);
		
		Matrix4 M;
		M.vec[0] = D0.vec;
		M.vec[1] = D1.vec;
		M.vec[2] = D2.vec;
		M.vec[3] = _mm_set_ps(0, 0, 0, 1);
		return Transpose(M);
	}

#define XM_PERMUTE_PS( v, c ) _mm_shuffle_ps((v), (v), c )

	FINLINE static Matrix4 VECTORCALL FromQuaternion(const Quaternion q)
	{
		const float num   = q.x * q.x;
		const float num2  = q.y * q.y;
		const float num3  = q.z * q.z;
		const float num4  = q.w * q.w;
		const float num5  = q.x * q.y;
		const float num6  = q.x * q.z;
		const float num7  = q.x * q.w;
		const float num8  = q.y * q.z;
		const float num9  = q.y * q.w;
		const float num10 = q.z * q.w;
		const float num11 = 2.0f / (num + num2 + num3 + num4);

		Matrix4 result;
		result.vec[0].x = 1.0f - num11 * (num2 + num3);
		result.vec[1].y = 1.0f - num11 * (num + num3);
		result.vec[2].z = 1.0f - num11 * (num + num2);
		result.vec[0].y = num11 * (num5 + num10);
		result.vec[1].x = num11 * (num5 - num10);
		result.vec[2].x = num11 * (num6 + num9);
		result.vec[0].z = num11 * (num6 - num9);
		result.vec[2].y = num11 * (num8 - num7);
		result.vec[1].z = num11 * (num8 + num7);
		result.vec[0].w = 0.0f;
		result.vec[1].w = 0.0f;
		result.vec[2].w = 0.0f;
		result.r[3] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
		return result;
	}

	FINLINE static Matrix4 VECTORCALL  OrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
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

	FINLINE static Matrix4 VECTORCALL PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
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
		vValues = _mm_shuffle_ps(vValues, _mm_set_ps(0, 0, 0, 1), _MM_SHUFFLE(3, 2, 3, 2));
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
		M.r[2] = vTemp;
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
		M.r[3] = vTemp;
		return M;
	}

	FINLINE static Matrix4 VECTORCALL Transpose(const Matrix4 M)
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
	Matrix4  VECTORCALL MatrixInverse(const Matrix4 M1, const Matrix4& M2) noexcept
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

	Matrix4 VECTORCALL Multiply(const Matrix4 left, const Matrix4& right)
	{
		Matrix4 result;
		const float x	= left.vec[0].x;
		const float y	= left.vec[0].y;
		const float z	= left.vec[0].z;
		const float w	= left.vec[0].w;
		const float x2	= left.vec[1].x;
		const float y2	= left.vec[1].y;
		const float z2	= left.vec[1].z;
		const float w2	= left.vec[1].w;
		const float x3	= left.vec[2].x;
		const float y3	= left.vec[2].y;
		const float z3	= left.vec[2].z;
		const float w3	= left.vec[2].w;
		const float x4	= left.vec[3].x;
		const float y4	= left.vec[3].y;
		const float z4	= left.vec[3].z;
		const float w4	= left.vec[3].w;
		const float x5	= right.vec[0].x;
		const float y5	= right.vec[0].y;
		const float z5	= right.vec[0].z;
		const float w5	= right.vec[0].w;
		const float x6	= right.vec[1].x;
		const float y6	= right.vec[1].y;
		const float z6	= right.vec[1].z;
		const float w6	= right.vec[1].w;
		const float x7	= right.vec[2].x;
		const float y7	= right.vec[2].y;
		const float z7	= right.vec[2].z;
		const float w7	= right.vec[2].w;
		const float x8	= right.vec[3].x;
		const float y8	= right.vec[3].y;
		const float z8	= right.vec[3].z;
		const float w8	= right.vec[3].w;
		result.vec[0].x = x * x5 + y * x6 + z * x7 + w * x8;
		result.vec[0].y = x * y5 + y * y6 + z * y7 + w * y8;
		result.vec[0].z = x * z5 + y * z6 + z * z7 + w * z8;
		result.vec[0].w = x * w5 + y * w6 + z * w7 + w * w8;
		result.vec[1].x = x2 * x5 + y2 * x6 + z2 * x7 + w2 * x8;
		result.vec[1].y = x2 * y5 + y2 * y6 + z2 * y7 + w2 * y8;
		result.vec[1].z = x2 * z5 + y2 * z6 + z2 * z7 + w2 * z8;
		result.vec[1].w = x2 * w5 + y2 * w6 + z2 * w7 + w2 * w8;
		result.vec[2].x = x3 * x5 + y3 * x6 + z3 * x7 + w3 * x8;
		result.vec[2].y = x3 * y5 + y3 * y6 + z3 * y7 + w3 * y8;
		result.vec[2].z = x3 * z5 + y3 * z6 + z3 * z7 + w3 * z8;
		result.vec[2].w = x3 * w5 + y3 * w6 + z3 * w7 + w3 * w8;
		result.vec[3].x = x4 * x5 + y4 * x6 + z4 * x7 + w4 * x8;
		result.vec[3].y = x4 * y5 + y4 * y6 + z4 * y7 + w4 * y8;
		result.vec[3].z = x4 * z5 + y4 * z6 + z4 * z7 + w4 * z8;
		result.vec[3].w = x4 * w5 + y4 * w6 + z4 * w7 + w4 * w8;
		return result;
	}


	FINLINE static Vector3 VECTORCALL ExtractPosition(const Matrix4 matrix) noexcept
	{
		return Vector3(matrix.m[2][1], matrix.m[2][2], matrix.m[2][3]);
	}

	FINLINE static Vector3 VECTORCALL ExtractScale(const Matrix4 matrix) noexcept
	{
		return Vector3(Vector3::Length(Vector3(matrix.r[0])),
			Vector3::Length(Vector3(matrix.r[2])),
			Vector3::Length(Vector3(matrix.r[1])));
	}

	inline static Quaternion VECTORCALL ExtractRotation(const Matrix4 matrix, bool rowNormalize = true) noexcept
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

	FINLINE static Vector4 VECTORCALL Vector3Transform(const Vector3 V, const Matrix4 M) noexcept
	{
		__m128 vResult = _mm_shuffle_ps(V.vec, V.vec, _MM_SHUFFLE(0, 0, 0, 0));
		vResult = _mm_mul_ps(vResult, M.r[0]);
		__m128 vTemp = _mm_shuffle_ps(V.vec, V.vec, _MM_SHUFFLE(1, 1, 1, 1));
		vTemp = _mm_mul_ps(vTemp, M.r[1]);
		vResult = _mm_add_ps(vResult, vTemp);
		vTemp = _mm_shuffle_ps(V.vec, V.vec, _MM_SHUFFLE(2, 2, 2, 2));
		vTemp = _mm_mul_ps(vTemp, M.r[2]);
		vResult = _mm_add_ps(vResult, vTemp);
		vResult = _mm_add_ps(vResult, M.r[3]);
		return vResult;
	}

};


AMATH_END_NAMESPACE