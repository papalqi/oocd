#include "Plane.h"
#include "Vector3.h"

oocd::Plane::Plane()
{
}

oocd::Plane::Plane(const Vector4& V) : Vector(V)
, W(V.W)
{
}

oocd::Plane::Plane(const Plane& P) : Vector(P)
, W(P.W)
{
}

oocd::Plane::Plane(Vector A, Vector B, Vector C) : Vector(((B - A) ^ (C - A)).GetSafeNormal())
{
	W = A | (Vector)(*this);
}

oocd::Plane::Plane(Vector InBase, const Vector &InNormal) : Vector(InNormal)
, W(InBase | InNormal)
{
}

oocd::Plane::Plane(Vector InNormal, float InW) : Vector(InNormal), W(InW)
{
}

oocd::Plane oocd::Plane::operator/=(float V)
{
	const float RV = 1.f / V;
	X *= RV; Y *= RV; Z *= RV; W *= RV;
	return *this;
}

oocd::Plane oocd::Plane::operator*=(const Plane& V)
{
	X *= V.X; Y *= V.Y; Z *= V.Z; W *= V.W;
	return *this;
}

oocd::Plane::Plane(float InX, float InY, float InZ, float InW) : Vector(InX, InY, InZ)
, W(InW)
{
}

oocd::Plane oocd::Plane::operator*=(float Scale)
{
	X *= Scale; Y *= Scale; Z *= Scale; W *= Scale;
	return *this;
}

oocd::Plane oocd::Plane::operator-=(const Plane& V)
{
	X -= V.X; Y -= V.Y; Z -= V.Z; W -= V.W;
	return *this;
}

oocd::Plane oocd::Plane::operator+=(const Plane& V)
{
	X += V.X; Y += V.Y; Z += V.Z; W += V.W;
	return *this;
}

oocd::Plane oocd::Plane::operator*(const Plane& V)
{
	return Plane(X * V.X, Y * V.Y, Z * V.Z, W * V.W);
}

oocd::Plane oocd::Plane::operator*(float Scale) const
{
	return Plane(X * Scale, Y * Scale, Z * Scale, W * Scale);
}

oocd::Plane oocd::Plane::operator/(float Scale) const
{
	const float RScale = 1.f / Scale;
	return Plane(X * RScale, Y * RScale, Z * RScale, W * RScale);
}

oocd::Plane oocd::Plane::operator-(const Plane& V) const
{
	return Plane(X - V.X, Y - V.Y, Z - V.Z, W - V.W);
}

oocd::Plane oocd::Plane::operator+(const Plane& V) const
{
	return Plane(X + V.X, Y + V.Y, Z + V.Z, W + V.W);
}

float oocd::Plane::operator|(const Plane& V) const
{
	return X * V.X + Y * V.Y + Z * V.Z + W * V.W;
}

bool oocd::Plane::Equals(const Plane& V, float Tolerance) const
{
	return (abs(X - V.X) < Tolerance) && (abs(Y - V.Y) < Tolerance) && (abs(Z - V.Z) < Tolerance) && (abs(W - V.W) < Tolerance);
}

bool oocd::Plane::operator!=(const Plane& V) const
{
	return (X != V.X) || (Y != V.Y) || (Z != V.Z) || (W != V.W);
}

bool oocd::Plane::operator==(const Plane& V) const
{
	return (X == V.X) && (Y == V.Y) && (Z == V.Z) && (W == V.W);
}

oocd::Plane oocd::Plane::Flip() const
{
	return Plane(-X, -Y, -Z, -W);
}

bool oocd::Plane::Normalize(float Tolerance)
{
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum > Tolerance)
	{
		const float Scale = sqrt(SquareSum);
		X *= Scale; Y *= Scale; Z *= Scale; W *= Scale;
		return true;
	}
	return false;
}

float oocd::Plane::PlaneDot(const Vector &P) const
{
	return X * P.X + Y * P.Y + Z * P.Z - W;
}
oocd::Matrix::Matrix(const Plane& InX, const Plane& InY, const Plane& InZ, const Plane& InW)
{
	M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = InX.W;
	M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = InY.W;
	M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = InZ.W;
	M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = InW.W;
}