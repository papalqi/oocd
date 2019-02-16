#include "Transform.h"

oocd::Transform::Transform() : MRotation(0.f, 0.f, 0.f, 1.f)
, Translation(0.f)
, Scale3D(Vector::OneVector)
{

}

oocd::Matrix oocd::Transform::ToMatrixWithScale() const
{
	Matrix OutMatrix;
	OutMatrix.M[3][0] = Translation.X;
	OutMatrix.M[3][1] = Translation.Y;
	OutMatrix.M[3][2] = Translation.Z;

	const float x2 = MRotation.X + MRotation.X;
	const float y2 = MRotation.Y + MRotation.Y;
	const float z2 = MRotation.Z + MRotation.Z;
	{
		const float xx2 = MRotation.X * x2;
		const float yy2 = MRotation.Y * y2;
		const float zz2 = MRotation.Z * z2;

		OutMatrix.M[0][0] = (1.0f - (yy2 + zz2)) * Scale3D.X;
		OutMatrix.M[1][1] = (1.0f - (xx2 + zz2)) * Scale3D.Y;
		OutMatrix.M[2][2] = (1.0f - (xx2 + yy2)) * Scale3D.Z;
	}
	{
		const float yz2 = MRotation.Y * z2;
		const float wx2 = MRotation.W * x2;

		OutMatrix.M[2][1] = (yz2 - wx2) * Scale3D.Z;
		OutMatrix.M[1][2] = (yz2 + wx2) * Scale3D.Y;
	}
	{
		const float xy2 = MRotation.X * y2;
		const float wz2 = MRotation.W * z2;

		OutMatrix.M[1][0] = (xy2 - wz2) * Scale3D.Y;
		OutMatrix.M[0][1] = (xy2 + wz2) * Scale3D.X;
	}
	{
		const float xz2 = MRotation.X * z2;
		const float wy2 = MRotation.W * y2;

		OutMatrix.M[2][0] = (xz2 + wy2) * Scale3D.Z;
		OutMatrix.M[0][2] = (xz2 - wy2) * Scale3D.X;
	}

	OutMatrix.M[0][3] = 0.0f;
	OutMatrix.M[1][3] = 0.0f;
	OutMatrix.M[2][3] = 0.0f;
	OutMatrix.M[3][3] = 1.0f;

	return OutMatrix;
}

oocd::Transform oocd::Transform::Inverse() const
{
	Quat   InvRotation = MRotation.Inverse();
	Vector InvScale3D = GetSafeScaleReciprocal(Scale3D);
	Vector InvTranslation = InvRotation * (InvScale3D * -Translation);

	return Transform(InvRotation, InvTranslation, InvScale3D);
}

oocd::Matrix oocd::Transform::ToMatrixNoScale() const
{
	Matrix OutMatrix;

	OutMatrix.M[3][0] = Translation.X;
	OutMatrix.M[3][1] = Translation.Y;
	OutMatrix.M[3][2] = Translation.Z;

	const float x2 = MRotation.X + MRotation.X;
	const float y2 = MRotation.Y + MRotation.Y;
	const float z2 = MRotation.Z + MRotation.Z;
	{
		const float xx2 = MRotation.X * x2;
		const float yy2 = MRotation.Y * y2;
		const float zz2 = MRotation.Z * z2;

		OutMatrix.M[0][0] = (1.0f - (yy2 + zz2));
		OutMatrix.M[1][1] = (1.0f - (xx2 + zz2));
		OutMatrix.M[2][2] = (1.0f - (xx2 + yy2));
	}
	{
		const float yz2 = MRotation.Y * z2;
		const float wx2 = MRotation.W * x2;

		OutMatrix.M[2][1] = (yz2 - wx2);
		OutMatrix.M[1][2] = (yz2 + wx2);
	}
	{
		const float xy2 = MRotation.X * y2;
		const float wz2 = MRotation.W * z2;

		OutMatrix.M[1][0] = (xy2 - wz2);
		OutMatrix.M[0][1] = (xy2 + wz2);
	}
	{
		const float xz2 = MRotation.X * z2;
		const float wy2 = MRotation.W * y2;

		OutMatrix.M[2][0] = (xz2 + wy2);
		OutMatrix.M[0][2] = (xz2 - wy2);
	}

	OutMatrix.M[0][3] = 0.0f;
	OutMatrix.M[1][3] = 0.0f;
	OutMatrix.M[2][3] = 0.0f;
	OutMatrix.M[3][3] = 1.0f;

	return OutMatrix;
}

void oocd::Transform::Blend(const Transform& Atom1, const Transform& Atom2, float Alpha)
{
	if (Alpha <= ZERO_ANIMWEIGHT_THRESH)
	{
		
		(*this) = Atom1;
	}
	else if (Alpha >= 1.f - ZERO_ANIMWEIGHT_THRESH)
	{
		// if blend is all the way for child2, then just copy its bone atoms
		(*this) = Atom2;
	}
	else
	{
		// Simple linear interpolation for translation and scale.
		Translation = Math::Lerp(Atom1.Translation, Atom2.Translation, Alpha);
		Scale3D = Math::Lerp(Atom1.Scale3D, Atom2.Scale3D, Alpha);
		MRotation = Quat::FastLerp(Atom1.MRotation, Atom2.MRotation, Alpha);

		// ..and renormalize
		MRotation.Normalize();
	}
}

void oocd::Transform::BlendWith(const Transform& OtherAtom, float Alpha)
{
	if (Alpha > ZERO_ANIMWEIGHT_THRESH)
	{
		if (Alpha >= 1.f - ZERO_ANIMWEIGHT_THRESH)
		{
			// if blend is all the way for child2, then just copy its bone atoms
			(*this) = OtherAtom;
		}
		else
		{
			// Simple linear interpolation for translation and scale.
			Translation = Math::Lerp(Translation, OtherAtom.Translation, Alpha);
			Scale3D = Math::Lerp(Scale3D, OtherAtom.Scale3D, Alpha);
			MRotation = Quat::FastLerp(MRotation, OtherAtom.MRotation, Alpha);

			// ..and renormalize
			MRotation.Normalize();
		}
	}
}

oocd::Transform::Transform(const Transform& InTransform) :
	MRotation(InTransform.MRotation),
	Translation(InTransform.Translation),
	Scale3D(InTransform.Scale3D)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const oocd::Rotator& InRotation, const Vector& InTranslation, const Vector& InScale3D /*= Vector::OneVector*/) : MRotation(InRotation),
Translation(InTranslation),
Scale3D(InScale3D)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const Quat& InRotation, const Vector& InTranslation, const Vector& InScale3D /*= Vector::OneVector*/) : MRotation(InRotation),
Translation(InTranslation),
Scale3D(InScale3D)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const oocd::Rotator& InRotation) : MRotation(InRotation),
Translation(Vector::ZeroVector),
Scale3D(Vector::OneVector)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const Quat& InRotation) : MRotation(InRotation),
Translation(Vector::ZeroVector),
Scale3D(Vector::OneVector)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const Vector& InTranslation) : MRotation(Quat::Identity),
Translation(InTranslation),
Scale3D(Vector::OneVector)
{
	DiagnosticCheckNaN_All();
}


//得到倒数，0是没有意义的，还是0
Vector Transform::GetSafeScaleReciprocal(const Vector& InScale, float Tolerance)
{
	Vector SafeReciprocalScale;
	if (Math::Abs(InScale.X) <= Tolerance)
	{
		SafeReciprocalScale.X = 0.f;
	}
	else
	{
		SafeReciprocalScale.X = 1 / InScale.X;
	}

	if (Math::Abs(InScale.Y) <= Tolerance)
	{
		SafeReciprocalScale.Y = 0.f;
	}
	else
	{
		SafeReciprocalScale.Y = 1 / InScale.Y;
	}

	if (Math::Abs(InScale.Z) <= Tolerance)
	{
		SafeReciprocalScale.Z = 0.f;
	}
	else
	{
		SafeReciprocalScale.Z = 1 / InScale.Z;
	}

	return SafeReciprocalScale;
}

