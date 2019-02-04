#pragma once


#include "Type.h"
#include "MathHelper.h"
#include "Vector3.h"

namespace oocd
{
	class Sphere
	{
	public:

		Vector Center;

		// radius
		float W;

	public:

		Sphere() { }

		Sphere(int32)
			: Center(0.0f, 0.0f, 0.0f)
			, W(0)
		{ }
		Sphere(Vector InV, float InW)
			: Center(InV)
			, W(InW)
		{ }
		 Sphere(const Vector* Pts, int32 Count);

	public:


		bool Equals(const Sphere& Sphere, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return Center.Equals(Sphere.Center, Tolerance) && Math::Abs(W - Sphere.W) <= Tolerance;
		}

		bool IsInside(const Sphere& Other, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			if (W > Other.W + Tolerance)
			{
				return false;
			}

			return (Center - Other.Center).SizeSquared() <= Math::Square(Other.W + Tolerance - W);
		}


		bool IsInside(const Vector& In, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return (Center - In).SizeSquared() <= Math::Square(W + Tolerance);
		}


		bool Intersects(const Sphere& Other, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return (Center - Other.Center).SizeSquared() <= Math::Square(Math::Max(0.f, Other.W + W + Tolerance));
		}

	
		 Sphere TransformBy(const Matrix& M) const;

	
		// Sphere TransformBy(const FTransform& M) const;

		
		 float GetVolume() const;

		
		 Sphere& operator+=(const Sphere& Other);

		
		Sphere operator+(const Sphere& Other) const
		{
			return Sphere(*this) += Other;
		}

	public:

	
	};
}

