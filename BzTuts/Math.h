#pragma once
#include <d3d12.h>
#include <DirectXMath.h>

using namespace DirectX;
struct Vertex {
	Vertex(float x, float y, float z, float r, float g, float b, float a);
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

#define PI 					(3.1415926535897932f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)

namespace oocd
{
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
		static  T Max3(const T A, const T B, const T C)
		{
			return max(max(A, B), C);
		}


		template< class T >
		static  T Min3(const T A, const T B, const T C)
		{
			return min(min(A, B), C);
		}

	
		template< class T >
		static  T Square(const T A)
		{
			return A * A;
		}
	};
}

