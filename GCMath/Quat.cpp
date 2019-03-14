#include "Quat.h"
#include"Vector.h"

using namespace oocd;
const Quat Quat::Identity(0, 0, 0, 1);

Quat::Quat(float InX, float InY, float InZ, float InW)
	: X(InX)
	, Y(InY)
	, Z(InZ)
	, W(InW)
{
}

Quat Quat::operator+(const Quat& Q) const
{
	return Quat(X + Q.X, Y + Q.Y, Z + Q.Z, W + Q.W);
}

Quat Quat::SlerpFullPath_NotNormalized(const Quat &quat1, const Quat &quat2, float Alpha)
{
	const float CosAngle = Math::Clamp(quat1 | quat2, -1.f, 1.f);
	const float Angle = Math::Acos(CosAngle);
	if (Math::Abs(Angle) < KINDA_SMALL_NUMBER)
	{
		return quat1;
	}

	const float SinAngle = Math::Sin(Angle);
	const float InvSinAngle = 1.f / SinAngle;

	const float Scale0 = Math::Sin((1.0f - Alpha)*Angle)*InvSinAngle;
	const float Scale1 = Math::Sin(Alpha*Angle)*InvSinAngle;

	return quat1 * Scale0 + quat2 * Scale1;
}

Quat Quat::Squad(const Quat& quat1, const Quat& tang1, const Quat& quat2, const Quat& tang2, float Alpha)
{
	const Quat Q1 = Quat::Slerp_NotNormalized(quat1, quat2, Alpha);
	const Quat Q2 = Quat::SlerpFullPath_NotNormalized(tang1, tang2, Alpha);
	const Quat Result = Quat::SlerpFullPath(Q1, Q2, 2.f * Alpha * (1.f - Alpha));

	return Result;
}

Quat Quat::SquadFullPath(const Quat& quat1, const Quat& tang1, const Quat& quat2, const Quat& tang2, float Alpha)
{
	const Quat Q1 = Quat::SlerpFullPath_NotNormalized(quat1, quat2, Alpha);
	const Quat Q2 = Quat::SlerpFullPath_NotNormalized(tang1, tang2, Alpha);
	const Quat Result = Quat::SlerpFullPath(Q1, Q2, 2.f * Alpha * (1.f - Alpha));

	return Result;
}

void Quat::CalcTangents(const Quat& PrevP, const Quat& P, const Quat& NextP, float Tension, Quat& OutTan)
{
}

Quat::Quat(oocd::Vector Axis, float AngleRad)
{
	const float half_a = 0.5f * AngleRad;
	float s, c;
	Math::SinCos(&s, &c, half_a);

	X = s * Axis.X;
	Y = s * Axis.Y;
	Z = s * Axis.Z;
	W = c;
}

Quat::Quat(const oocd::Rotator& R)
{
	*this = R.Quaternion();
}
Quat Rotator::Quaternion() const
{
	const float DEG_TO_RAD = PI / (180.f);
	const float DIVIDE_BY_2 = DEG_TO_RAD / 2.f;
	float SP, SY, SR;
	float CP, CY, CR;

	Math::SinCos(&SP, &CP, Pitch*DIVIDE_BY_2);
	Math::SinCos(&SY, &CY, Yaw*DIVIDE_BY_2);
	Math::SinCos(&SR, &CR, Roll*DIVIDE_BY_2);

	Quat RotationQuat;
	RotationQuat.X = CR * SP*SY - SR * CP*CY;
	RotationQuat.Y = -CR * SP*CY - SR * CP*SY;
	RotationQuat.Z = CR * CP*SY - SR * SP*CY;
	RotationQuat.W = CR * CP*CY + SR * SP*SY;
	return RotationQuat;
}
Quat::Quat(const Matrix& M)
{
	if (M.GetScaledAxis(Axis::X).IsNearlyZero() || M.GetScaledAxis(Axis::Y).IsNearlyZero() || M.GetScaledAxis(Axis::Z).IsNearlyZero())
	{
		*this = Quat::Identity;
		return;
	}

	//const MeReal *const t = (MeReal *) tm;
	float	s;

	// Check diagonal (trace)
	const float tr = M.M[0][0] + M.M[1][1] + M.M[2][2];

	if (tr > 0.0f)
	{
		float InvS = Math::InvSqrt(tr + 1.f);
		this->W = 0.5f * (1.f / InvS);
		s = 0.5f * InvS;

		this->X = (M.M[1][2] - M.M[2][1]) * s;
		this->Y = (M.M[2][0] - M.M[0][2]) * s;
		this->Z = (M.M[0][1] - M.M[1][0]) * s;
	}
	else
	{
		// diagonal is negative
		int32 i = 0;

		if (M.M[1][1] > M.M[0][0])
			i = 1;

		if (M.M[2][2] > M.M[i][i])
			i = 2;

		static const int32 nxt[3] = { 1, 2, 0 };
		const int32 j = nxt[i];
		const int32 k = nxt[j];

		s = M.M[i][i] - M.M[j][j] - M.M[k][k] + 1.0f;

		float InvS = Math::InvSqrt(s);

		float qt[4];
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (M.M[j][k] - M.M[k][j]) * s;
		qt[j] = (M.M[i][j] + M.M[j][i]) * s;
		qt[k] = (M.M[i][k] + M.M[k][i]) * s;

		this->X = qt[0];
		this->Y = qt[1];
		this->Z = qt[2];
		this->W = qt[3];
	}
}

Quat::Quat(const Quat& Q)
	: X(Q.X)
	, Y(Q.Y)
	, Z(Q.Z)
	, W(Q.W)
{ }

Quat Quat::Slerp_NotNormalized(const Quat &Quat1, const Quat &Quat2, float Slerp)
{
	const float RawCosom =
		Quat1.X * Quat2.X +
		Quat1.Y * Quat2.Y +
		Quat1.Z * Quat2.Z +
		Quat1.W * Quat2.W;

	// Unaligned quats - compensate, results in taking shorter route.
	const float Cosom = Math::FloatSelect(RawCosom, RawCosom, -RawCosom);

	float Scale0, Scale1;

	if (Cosom < 0.9999f)
	{
		const float Omega = Math::Acos(Cosom);
		const float InvSin = 1.f / Math::Sin(Omega);
		Scale0 = Math::Sin((1.f - Slerp) * Omega) * InvSin;
		Scale1 = Math::Sin(Slerp * Omega) * InvSin;
	}
	else
	{
		// Use linear interpolation.
		Scale0 = 1.0f - Slerp;
		Scale1 = Slerp;
	}

	// In keeping with our flipped Cosom:
	Scale1 = Math::FloatSelect(RawCosom, Scale1, -Scale1);

	Quat Result;

	Result.X = Scale0 * Quat1.X + Scale1 * Quat2.X;
	Result.Y = Scale0 * Quat1.Y + Scale1 * Quat2.Y;
	Result.Z = Scale0 * Quat1.Z + Scale1 * Quat2.Z;
	Result.W = Scale0 * Quat1.W + Scale1 * Quat2.W;

	return Result;
}

Quat Quat::FastBilerp(const Quat& P00, const Quat& P10, const Quat& P01, const Quat& P11, float FracX, float FracY)
{
	return Quat::FastLerp(
		Quat::FastLerp(P00, P10, FracX),
		Quat::FastLerp(P01, P11, FracX),
		FracY
	);
}

Quat Quat::FastLerp(const Quat& A, const Quat& B, const float Alpha)
{
	const float DotResult = (A | B);
	const float Bias = Math::FloatSelect(DotResult, 1.0f, -1.0f);
	return (B * Alpha) + (A * (Bias * (1.f - Alpha)));
}

float Quat::ErrorAutoNormalize(const Quat& A, const Quat& B)
{
	Quat Q1 = A;
	Q1.Normalize();

	Quat Q2 = B;
	Q2.Normalize();

	return Quat::Error(Q1, Q2);
}

oocd::Quat oocd::Quat::operator+=(const Quat& Q)
{
	this->X += Q.X;
	this->Y += Q.Y;
	this->Z += Q.Z;
	this->W += Q.W;

	return *this;
}

oocd::Quat oocd::Quat::operator-(const Quat& Q) const
{
	return Quat(X - Q.X, Y - Q.Y, Z - Q.Z, W - Q.W);
}

bool oocd::Quat::Equals(const Quat& Q, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	return (Math::Abs(X - Q.X) <= Tolerance && Math::Abs(Y - Q.Y) <= Tolerance && Math::Abs(Z - Q.Z) <= Tolerance && Math::Abs(W - Q.W) <= Tolerance)
		|| (Math::Abs(X + Q.X) <= Tolerance && Math::Abs(Y + Q.Y) <= Tolerance && Math::Abs(Z + Q.Z) <= Tolerance && Math::Abs(W + Q.W) <= Tolerance);
}

bool oocd::Quat::IsIdentity(float Tolerance /*= SMALL_NUMBER*/) const
{
	return Equals(Quat::Identity, Tolerance);
}

oocd::Quat oocd::Quat::operator*(const Quat& Q) const
{
	Quat Result;
	VectorQuaternionMultiply(&Result, this, &Q);
	return Result;
}

oocd::Quat oocd::Quat::operator*=(const Quat& Q)
{
	return *this;
}

oocd::Vector Quat::operator*(const oocd::Vector& V) const
{
	return RotateVector(V);
}

oocd::Matrix oocd::Quat::operator*(const Matrix& M) const
{
	Matrix Result;
	Quat VT, VR;
	Quat Inv = Inverse();
	for (int32 I = 0; I < 4; ++I)
	{
		Quat VQ(M.M[I][0], M.M[I][1], M.M[I][2], M.M[I][3]);
		VectorQuaternionMultiply(&VT, this, &VQ);
		VectorQuaternionMultiply(&VR, &VT, &Inv);
		Result.M[I][0] = VR.X;
		Result.M[I][1] = VR.Y;
		Result.M[I][2] = VR.Z;
		Result.M[I][3] = VR.W;
	}

	return Result;
}

oocd::Quat oocd::Quat::operator*=(const float Scale)
{
	X *= Scale;
	Y *= Scale;
	Z *= Scale;
	W *= Scale;

	return *this;
}

oocd::Quat oocd::Quat::operator*(const float Scale) const
{
	return Quat(Scale * X, Scale * Y, Scale * Z, Scale * W);
}

oocd::Quat oocd::Quat::operator/=(const float Scale)
{
	const float Recip = 1.0f / Scale;
	X *= Recip;
	Y *= Recip;
	Z *= Recip;
	W *= Recip;
	return *this;
}

oocd::Quat oocd::Quat::operator/(const float Scale) const
{
	const float Recip = 1.0f / Scale;
	return Quat(X * Recip, Y * Recip, Z * Recip, W * Recip);
}

bool oocd::Quat::operator!=(const Quat& Q) const
{
	return X != Q.X || Y != Q.Y || Z != Q.Z || W != Q.W;
}

bool oocd::Quat::operator==(const Quat& Q) const
{
	return X == Q.X && Y == Q.Y && Z == Q.Z && W == Q.W;
}

float oocd::Quat::operator|(const Quat& Q) const
{
	return X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
}

oocd::Quat oocd::Quat::MakeFromEuler(const oocd::Vector& Euler)
{
	return oocd::Rotator(Euler.Y, Euler.Z, Euler.X).Quaternion();
}

oocd::Vector oocd::Quat::Euler() const
{
	return Rotator().Euler();
}

void oocd::Quat::Normalize(float Tolerance /*= SMALL_NUMBER*/)
{
	const float SquareSum = X * X + Y * Y + Z * Z + W * W;

	if (SquareSum >= Tolerance)
	{
		const float Scale = Math::InvSqrt(SquareSum);

		X *= Scale;
		Y *= Scale;
		Z *= Scale;
		W *= Scale;
	}
	else
	{
		*this = Quat::Identity;
	}
}

oocd::Quat oocd::Quat::GetNormalized(float Tolerance /*= SMALL_NUMBER*/) const
{
	Quat Result(*this);
	Result.Normalize(Tolerance);
	return Result;
}

bool oocd::Quat::IsNormalized() const
{
	return (Math::Abs(1.f - SizeSquared()) < THRESH_QUAT_NORMALIZED);
}

float oocd::Quat::Size() const
{
	return Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
}

float oocd::Quat::SizeSquared() const
{
	return (X * X + Y * Y + Z * Z + W * W);
}

float oocd::Quat::GetAngle() const
{
	return 2.f * Math::Acos(W);
}

void oocd::Quat::ToAxisAndAngle(oocd::Vector& Axis, float& Angle) const
{
	Angle = GetAngle();
	Axis = GetRotationAxis();
}

void oocd::Quat::ToSwingTwist(const oocd::Vector& InTwistAxis, Quat& OutSwing, Quat& OutTwist) const
{
	oocd::Vector Projection = oocd::Vector::DotProduct(InTwistAxis, oocd::Vector(X, Y, Z)) * InTwistAxis;

	// Twist quaternion
	OutTwist = Quat(Projection.X, Projection.Y, Projection.Z, W);

	// Singularity close to 180deg
	if (OutTwist.SizeSquared() == 0.0f)
	{
		OutTwist = Quat::Identity;
	}
	else
	{
		OutTwist.Normalize();
	}

	// Set swing
	OutSwing = *this * OutTwist.Inverse();
}

oocd::Vector oocd::Quat::RotateVector(oocd::Vector V) const
{
	const oocd::Vector Q(X, Y, Z);
	const oocd::Vector T = 2.f * oocd::Vector::CrossProduct(Q, V);
	const oocd::Vector Result = V + (W * T) + oocd::Vector::CrossProduct(Q, T);
	return Result;
}

oocd::Vector oocd::Quat::UnrotateVector(oocd::Vector V) const
{
	const oocd::Vector Q(-X, -Y, -Z); // Inverse
	const oocd::Vector T = 2.f * oocd::Vector::CrossProduct(Q, V);
	const oocd::Vector Result = V + (W * T) + oocd::Vector::CrossProduct(Q, T);
	return Result;
}

oocd::Quat oocd::Quat::Inverse() const
{
	return Quat(-X, -Y, -Z, W);
}

void oocd::Quat::EnforceShortestArcWith(const Quat& OtherQuat)
{
	const float DotResult = (OtherQuat | *this);
	const float Bias = Math::FloatSelect(DotResult, 1.0f, -1.0f);

	X *= Bias;
	Y *= Bias;
	Z *= Bias;
	W *= Bias;
}

oocd::Quat oocd::Quat::operator-=(const Quat& Q)
{
	this->X -= Q.X;
	this->Y -= Q.Y;
	this->Z -= Q.Z;
	this->W -= Q.W;

	return *this;
}

oocd::Vector oocd::Quat::GetAxisX() const
{
	return RotateVector(oocd::Vector(1.f, 0.f, 0.f));
}

oocd::Vector oocd::Quat::GetAxisY() const
{
	return RotateVector(oocd::Vector(0.f, 1.f, 0.f));
}

oocd::Vector oocd::Quat::GetAxisZ() const
{
	return RotateVector(oocd::Vector(0.f, 0.f, 1.f));
}

oocd::Vector oocd::Quat::GetForwardVector() const
{
	return GetAxisX();
}

oocd::Vector oocd::Quat::GetRightVector() const
{
	return GetAxisY();
}

oocd::Vector oocd::Quat::GetUpVector() const
{
	return GetAxisZ();
}

oocd::Vector oocd::Quat::Vector() const
{
	return GetAxisX();
}

oocd::Rotator oocd::Quat::Rotator() const
{
	const float SingularityTest = Z * X - W * Y;
	const float YawY = 2.f*(W*Z + X * Y);
	const float YawX = (1.f - 2.f*(Math::Square(Y) + Math::Square(Z)));

	const float SINGULARITY_THRESHOLD = 0.4999995f;
	const float RAD_TO_DEG = (180.f) / PI;
	oocd::Rotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = -90.f;
		RotatorFromQuat.Yaw = Math::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = oocd::Rotator::NormalizeAxis(-RotatorFromQuat.Yaw - (2.f * Math::Atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = 90.f;
		RotatorFromQuat.Yaw = Math::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = oocd::Rotator::NormalizeAxis(RotatorFromQuat.Yaw - (2.f * Math::Atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		RotatorFromQuat.Pitch = Math::FastAsin(2.f*(SingularityTest)) * RAD_TO_DEG;
		RotatorFromQuat.Yaw = Math::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = Math::Atan2(-2.f*(W*X + Y * Z), (1.f - 2.f*(Math::Square(X) + Math::Square(Y)))) * RAD_TO_DEG;
	}

	return RotatorFromQuat;
}

oocd::Vector oocd::Quat::GetRotationAxis() const
{
	const float S = Math::Sqrt(Math::Max(1.f - (W * W), 0.f));

	if (S >= 0.0001f)
	{
		return oocd::Vector(X / S, Y / S, Z / S);
	}

	return oocd::Vector(1.f, 0.f, 0.f);
}

float oocd::Quat::AngularDistance(const Quat& Q) const
{
	float InnerProd = X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
	return Math::Acos((2 * InnerProd * InnerProd) - 1.f);
}

bool oocd::Quat::ContainsNaN() const
{
	return (!Math::IsFinite(X) ||
		!Math::IsFinite(Y) ||
		!Math::IsFinite(Z) ||
		!Math::IsFinite(W)
		);
}

float oocd::Quat::Error(const Quat& Q1, const Quat& Q2)
{
	const float cosom = Math::Abs(Q1.X * Q2.X + Q1.Y * Q2.Y + Q1.Z * Q2.Z + Q1.W * Q2.W);
	return (Math::Abs(cosom) < 0.9999999f) ? Math::Acos(cosom)*(1.f / PI) : 0.0f;
}