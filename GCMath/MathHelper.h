﻿#pragma once
#include <stdlib.h>
#include <math.h>
#include <DirectXMath.h>
#include"MathBase.h"

//计算常量
#define PI 					(3.1415926535897932f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)

#define INV_PI			(0.31830988618f)
#define HALF_PI			(1.57079632679f)

#define DELTA			(0.00001f)
using namespace std;
using namespace DirectX;
namespace oocd
{
	class Vector4;
	class Plane;
	class Matrix;
	//如果Comparand>0，我们选择ValueGEZero，否则ValueLTZero
	template <typename T>
	T Select(T Comparand, T ValueGEZero, T ValueLTZero)
	{
		return Comparand >= 0.f ? ValueGEZero : ValueLTZero;
	}

	enum Axis
	{
		None,
		X,
		Y,
		Z
	};

	class Math :public MathBase
	{
	public:
		template< class T >
		static   T Max(const T A, const T B)
		{
			return (A >= B) ? A : B;
		}

		/** Returns lower value in a generic way */
		template< class T >
		static   T Min(const T A, const T B)
		{
			return (A <= B) ? A : B;
		}

		//选择三个中最大的哪一个
		template< class T >
		static  T Max3(const T A, const T B, const T C)
		{
			return max(max(A, B), C);
		}

		//选择三个中最小的哪一个
		template< class T >
		static  T Min3(const T A, const T B, const T C)
		{
			return min(min(A, B), C);
		}

		//计算平方
		template< class T >
		static  T Square(const T A)
		{
			return A * A;
		}
		//应用牛顿迭代法计算平方分之一
		//todo:弄清楚牛顿迭代法
		static float InvSqrt(float F);
		static float Sqrt(float Value) { return sqrtf(Value); }
		static  bool IsFinite(float A) { return isfinite(A); }
		template< class T >
		static  T Abs(const T A)
		{
			return (A >= (T)0) ? A : -A;
		}
		static  float GridSnap(float Location, float Grid)
		{
			if (Grid == 0.f)	return Location;
			else
			{
				return (FloorToFloat(float((Location + 0.5*Grid) / Grid)*Grid));
			}
		}
		//计算sin和cos
		static  void SinCos(float* ScalarSin, float* ScalarCos, float  Value)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = (INV_PI*0.5f)*Value;
			if (Value >= 0.0f)
			{
				quotient = (float)((int)(quotient + 0.5f));
			}
			else
			{
				quotient = (float)((int)(quotient - 0.5f));
			}
			float y = Value - (2.0f*PI)*quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > HALF_PI)
			{
				y = PI - y;
				sign = -1.0f;
			}
			else if (y < -HALF_PI)
			{
				y = -PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			*ScalarCos = sign * p;
		}
		template<class T>
		//将度数转化为弧度
		static  auto DegreesToRadians(T const& DegVal) -> decltype(DegVal * (PI / 180.f))
		{
			return DegVal * (PI / 180.f);
		}
		static  bool IsNearlyEqual(float A, float B, float ErrorTolerance = SMALL_NUMBER)
		{
			return Abs<float>(A - B) <= ErrorTolerance;
		}
	};

	//使用的是DirectXMath的计算方法，并不要知道我自己，并没有进行检测
	void VectorMatrixMultiply(Matrix* Result, const Matrix* Matrix1, const Matrix* Matrix2);
	//
	void VectorMatrixInverse(void* DstMatrix, const void* SrcMatrix);
}
