#include "Vector4.h"

using namespace oocd;

const oocd::Vector4 oocd::Vector4::ZeroVector(0,0,0,0);

const oocd::Vector4 oocd::Vector4::ZeroAndUnitVector(0,0,0,1);

oocd::Vector4::Vector4(const Vector& InVector, float InW /*= 1.0f*/)
	: X(InVector.X)
	, Y(InVector.Y)
	, Z(InVector.Z)
	, W(InW)
{
}

float& oocd::Vector4::operator[](int ComponentIndex)
{
	return (&X)[ComponentIndex];
}

Vector4 oocd::Vector4::operator-() const
{
	return Vector4(-X, -Y, -Z, -W);
}

Vector4 oocd::Vector4::operator+(const Vector4 & V) const
{
	return Vector4(X + V.X, Y + V.Y, Z + V.Z, W + V.W);
}

Vector4 oocd::Vector4::operator+=(const Vector4 & V)
{
	X += V.X; Y += V.Y; Z += V.Z; W += V.W;
	return *this;
}

Vector4 oocd::Vector4::operator-(const Vector4 & V) const
{
	return Vector4(X - V.X, Y - V.Y, Z - V.Z, W - V.W);
}

Vector4 oocd::Vector4::operator-=(const Vector4 & V)
{
	X -= V.X; Y -= V.Y; Z -= V.Z; W -= V.W;
	return *this;
}

Vector4 oocd::Vector4::operator*(float Scale) const
{
	return Vector4(X * Scale, Y * Scale, Z * Scale, W * Scale);
}

Vector4 oocd::Vector4::operator/(float Scale) const
{
	const float RScale = 1.f / Scale;
	return Vector4(X * RScale, Y * RScale, Z * RScale, W * RScale);
}

Vector4 oocd::Vector4::operator/(const Vector4 & V) const
{
	return Vector4(X / V.X, Y / V.Y, Z / V.Z, W / V.W);
}

Vector4 oocd::Vector4::operator*(const Vector4 & V) const
{
	return Vector4(X * V.X, Y * V.Y, Z * V.Z, W * V.W);
}

Vector4 oocd::Vector4::operator*=(const Vector4 & V)
{
	X *= V.X; Y *= V.Y; Z *= V.Z; W *= V.W;
	return *this;
}

Vector4 oocd::Vector4::operator/=(const Vector4 & V)
{
	X /= V.X; Y /= V.Y; Z /= V.Z; W /= V.W;
	return *this;
}

Vector4 oocd::Vector4::operator*=(float S)
{
	X *= S; Y *= S; Z *= S; W *= S;
	return *this;
}

oocd::Vector4::Vector4(Vector2D InXY, Vector2D InZW)
	: X(InXY.X)
	, Y(InXY.Y)
	, Z(InZW.X)
	, W(InZW.Y)
{
}

oocd::Vector4::Vector4(float InX /*= 0.0f*/, float InY /*= 0.0f*/, float InZ /*= 0.0f*/, float InW /*= 1.0f*/)
	: X(InX)
	, Y(InY)
	, Z(InZ)
	, W(InW)
{
}

bool oocd::Vector4::operator==(const Vector4 & V) const
{
	return ((X == V.X) && (Y == V.Y) && (Z == V.Z) && (W == V.W));
}

bool oocd::Vector4::operator!=(const Vector4 & V) const
{
	return ((X != V.X) || (Y != V.Y) || (Z != V.Z) || (W != V.W));
}

Vector4 oocd::Vector4::operator^(const Vector4 & V) const
{
	return Vector4(
		Y * V.Z - Z * V.Y,
		Z * V.X - X * V.Z,
		X * V.Y - Y * V.X,
		0.0f
	);
}

float & oocd::Vector4::Component(int Index)
{
	return (&X)[Index];
}

const float & oocd::Vector4::Component(int Index) const
{
	return (&X)[Index];
}

bool oocd::Vector4::Equals(const Vector4 & V, float Tolerance) const
{
	return abs(X - V.X) <= Tolerance && abs(Y - V.Y) <= Tolerance && abs(Z - V.Z) <= Tolerance && abs(W - V.W) <= Tolerance;
}

bool oocd::Vector4::IsUnit3(float LengthSquaredTolerance) const
{
	return abs(1.0f - SizeSquared3()) < LengthSquaredTolerance;
}

Vector4 oocd::Vector4::GetSafeNormal(float Tolerance) const
{
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum > Tolerance)
	{
		const float Scale = sqrt(SquareSum);
		return Vector4(X*Scale, Y*Scale, Z*Scale, 0.0f);
	}
	return Vector4(0.f);
}

Vector4 oocd::Vector4::GetUnsafeNormal3() const
{
	const float Scale = sqrt(X*X + Y * Y + Z * Z);
	return Vector4(X*Scale, Y*Scale, Z*Scale, 0.0f);
}

void oocd::Vector4::Set(float InX, float InY, float InZ, float InW)
{
	X = InX;
	Y = InY;
	Z = InZ;
	W = InW;
}

float oocd::Vector4::Size3() const
{
	return sqrt(X*X + Y * Y + Z * Z);
}

float oocd::Vector4::SizeSquared3() const
{
	return X * X + Y * Y + Z * Z;
}

float oocd::Vector4::Size() const
{
	return sqrt(X*X + Y * Y + Z * Z + W * W);
}

float oocd::Vector4::SizeSquared() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

bool oocd::Vector4::ContainsNaN() const
{
	//todo
	return false;
}

bool oocd::Vector4::IsNearlyZero3(float Tolerance) const
{
	return
		abs(X) <= Tolerance
		&& abs(Y) <= Tolerance
		&& abs(Z) <= Tolerance;
}

Vector4 oocd::Vector4::Reflect3(const Vector4 & Normal) const
{
	return 2.0f * Dot3(*this, Normal) * Normal - *this;
}

void oocd::Vector4::FindBestAxisVectors3(Vector4 & Axis1, Vector4 & Axis2) const
{
	const float NX = abs(X);
	const float NY = abs(Y);
	const float NZ = abs(Z);

	// Find best basis vectors.
	if (NZ > NX && NZ > NY)	Axis1 = Vector4(1, 0, 0);
	else					Axis1 = Vector4(0, 0, 1);

	Axis1 = (Axis1 - *this * Dot3(Axis1, *this)).GetSafeNormal();
	Axis2 = Axis1 ^ *this;
}