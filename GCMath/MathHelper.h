#pragma once
#include <stdlib.h>
#include <math.h>
#include <DirectXMath.h>

//计算常量
#define PI 					(3.1415926535897932f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)


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

	struct Math
	{
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
		static  bool IsFinite( float A ) { return isfinite(A); }
	};




	//使用的是DirectXMath的计算方法，并不要知道我自己，并没有进行检测
	void VectorMatrixMultiply(Matrix* Result, const Matrix* Matrix1, const Matrix* Matrix2);
	//
	void VectorMatrixInverse(void* DstMatrix, const void* SrcMatrix);
	
}

