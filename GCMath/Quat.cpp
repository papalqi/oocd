#include "Quat.h"



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
	const Quat InvP = P.Inverse();
	const Quat Part1 = (InvP * PrevP).Log();
	const Quat Part2 = (InvP * NextP).Log();

	const Quat PreExp = (Part1 + Part2) * -0.5f;

	OutTan = P * PreExp.Exp();
}

Quat::Quat(Vector Axis, float AngleRad)
{
	const float half_a = 0.5f * AngleRad;
	float s, c;
	Math::SinCos(&s, &c, half_a);

	X = s * Axis.X;
	Y = s * Axis.Y;
	Z = s * Axis.Z;
	W = c;
}

Quat::Quat(const Rotator& R)
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

Quat Quat::FindBetweenVectors(const Vector& A, const Vector& B)
{
	const float NormAB = Math::Sqrt(A.SizeSquared() * B.SizeSquared());
	return FindBetween_Helper(A, B, NormAB);
}

float oocd::Quat::Error(const Quat& Q1, const Quat& Q2)
{
	const float cosom = Math::Abs(Q1.X * Q2.X + Q1.Y * Q2.Y + Q1.Z * Q2.Z + Q1.W * Q2.W);
	return (Math::Abs(cosom) < 0.9999999f) ? Math::Acos(cosom)*(1.f / PI) : 0.0f;
}
