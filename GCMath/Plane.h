#pragma once

#include"MathHelper.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
namespace oocd
{
	class Matrix;
	class Plane
		: public Vector
	{
	public:

		float W;

	public:

		Plane();

		//拷贝构造
		Plane(const Plane& P);

		Plane(const Vector4& V);

		Plane(float InX, float InY, float InZ, float InW);
		Plane(Vector InNormal, float InW);

		Plane(Vector InBase, const Vector &InNormal);

		Plane(Vector A, Vector B, Vector C);

		//计算平面和点之间的距离
		float PlaneDot(const Vector &P) const;

		//标准化
		bool Normalize(float Tolerance = SMALL_NUMBER);

		//得到反转
		Plane Flip() const;

		//变换，通过Matrix
		Plane TransformBy(const Matrix& M) const;

		//变换，得到伴随矩阵
		Plane TransformByUsingAdjointT(const Matrix& M, float DetM, const Matrix& TA) const;

		//是否完全相等
		bool operator==(const Plane& V) const;

		//是否不相等
		bool operator!=(const Plane& V) const;

		//是否相等
		bool Equals(const Plane& V, float Tolerance = KINDA_SMALL_NUMBER) const;

		//dot
		float operator|(const Plane& V) const;

		//+
		Plane operator+(const Plane& V) const;

		//-
		Plane operator-(const Plane& V) const;

		// 除法
		Plane operator/(float Scale) const;

		//乘法
		Plane operator*(float Scale) const;

		//想乘
		Plane operator*(const Plane& V);

		//+=
		Plane operator+=(const Plane& V);

		//-=
		Plane operator-=(const Plane& V);

		//*=
		Plane operator*=(float Scale);

		//*=
		Plane operator*=(const Plane& V);

		Plane operator/=(float V);
	};
}

/* FMath inline functions
	 *****************************************************************************/
namespace oocd
{
	//得到光线和平面的交点
	inline Vector RayPlaneIntersection(const Vector& RayOrigin, const Vector& RayDirection, const Plane& plane)
	{
		const Vector PlaneNormal = Vector(plane.X, plane.Y, plane.Z);
		const Vector PlaneOrigin = PlaneNormal * plane.W;

		const float Distance = Vector::DotProduct((PlaneOrigin - RayOrigin), PlaneNormal) / Vector::DotProduct(RayDirection, PlaneNormal);
		return RayOrigin + RayDirection * Distance;
	}

	//得到平面和直线的交点
	inline Vector LinePlaneIntersection
	(
		const Vector &Point1,
		const Vector &Point2,
		const Plane  &Plane
	)
	{
		return
			Point1
			+ (Point2 - Point1)
			*	((Plane.W - (Point1 | Plane)) / ((Point2 - Point1) | Plane));
	}
	//计算三个plane 的交点
	inline bool IntersectPlanes3(Vector& I, const Plane& P1, const Plane& P2, const Plane& P3)
	{
		// Compute determinant, the triple product P1|(P2^P3)==(P1^P2)|P3.
		const float Det = (P1 ^ P2) | P3;
		if (Math::Square(Det) < Math::Square(0.001f))
		{
			// Degenerate.
			I = Vector::ZeroVector;
			return 0;
		}
		else
		{
			// Compute the intersection point, guaranteed valid if determinant is nonzero.
			I = (P1.W*(P2^P3) + P2.W*(P3^P1) + P3.W*(P1^P2)) / Det;
		}
		return 1;
	}

	inline bool IntersectPlanes2(Vector& I, Vector& D, const Plane& P1, const Plane& P2)
	{
		// Compute line direction, perpendicular to both plane normals.
		D = P1 ^ P2;
		const float DD = D.SizeSquared();
		if (DD < Math::Square(0.001f))
		{
			// Parallel or nearly parallel planes.
			D = I = Vector::ZeroVector;
			return 0;
		}
		else
		{
			// Compute intersection.
			I = (P1.W*(P2^D) + P2.W*(D^P1)) / DD;
			D.Normalize();
			return 1;
		}
	}

	/* Vector inline functions
	 *****************************************************************************/

	inline Vector MirrorByPlane(const Plane& plane, const Vector V)
	{
		return V - plane * (2.f * plane.PlaneDot(V));
	}

	inline Vector PointPlaneProject(const Vector& Point, const Plane& plane)
	{
		//Find the distance of X from the plane
		//Add the distance back along the normal from the point
		return Point - plane.PlaneDot(Point) * plane;
	}

	inline Vector PointPlaneProject(const Vector& Point, const Vector& A, const Vector& B, const Vector& C)
	{
		//Compute the plane normal from ABC
		Plane plane(A, B, C);

		//Find the distance of X from the plane
		//Add the distance back along the normal from the point
		return Point - plane.PlaneDot(Point) * plane;
	}
	inline Plane Plane::TransformBy(const Matrix& M) const
	{
		const Matrix tmpTA = M.TransposeAdjoint();
		const float DetM = M.Determinant();
		return this->TransformByUsingAdjointT(M, DetM, tmpTA);
	}

	inline bool MakeFrustumPlane(float A, float B, float C, float D, Plane& OutPlane)
	{
		const float	LengthSquared = A * A + B * B + C * C;
		if (LengthSquared > DELTA*DELTA)
		{
			const float	InvLength = Math::InvSqrt(LengthSquared);
			OutPlane = Plane(-A * InvLength, -B * InvLength, -C * InvLength, D * InvLength);
			return 1;
		}
		else
			return 0;
	}
}
