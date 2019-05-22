#include "Transform.h"
using namespace oocd;
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


bool Transform::AnyHasNegativeScale(const Vector& InScale3D, const  Vector& InOtherScale3D)
{
	return  (InScale3D.X < 0.f || InScale3D.Y < 0.f || InScale3D.Z < 0.f
		|| InOtherScale3D.X < 0.f || InOtherScale3D.Y < 0.f || InOtherScale3D.Z < 0.f);
}

/** Scale the translation part of the Transform by the supplied vector. */
void Transform::ScaleTranslation(const Vector& InScale3D)
{
	Translation *= InScale3D;

	DiagnosticCheckNaN_Translate();
}


void Transform::ScaleTranslation(const float& Scale)
{
	Translation *= Scale;

	DiagnosticCheckNaN_Translate();
}


// this function is from matrix, and all it does is to normalize rotation portion
void Transform::RemoveScaling(float Tolerance/*=SMALL_NUMBER*/)
{
	Scale3D = Vector(1, 1, 1);
	MRotation.Normalize();

	DiagnosticCheckNaN_Rotate();
	DiagnosticCheckNaN_Scale3D();
}

void Transform::MultiplyUsingMatrixWithScale(Transform* OutTransform, const Transform* A, const Transform* B)
{
	// the goal of using M is to get the correct orientation
	// but for translation, we still need scale
	ConstructTransformFromMatrixWithDesiredScale(A->ToMatrixWithScale(), B->ToMatrixWithScale(), A->Scale3D*B->Scale3D, *OutTransform);
}

void Transform::ConstructTransformFromMatrixWithDesiredScale(const Matrix& AMatrix, const Matrix& BMatrix, const Vector& DesiredScale, Transform& OutTransform)
{
	// the goal of using M is to get the correct orientation
	// but for translation, we still need scale
	Matrix M = AMatrix * BMatrix;
	M.RemoveScaling();

	// apply negative scale back to axes
	Vector SignedScale = DesiredScale.GetSignVector();

	M.SetAxis(0, SignedScale.X * M.GetScaledAxis(Axis::X));
	M.SetAxis(1, SignedScale.Y * M.GetScaledAxis(Axis::Y));
	M.SetAxis(2, SignedScale.Z * M.GetScaledAxis(Axis::Z));

	// @note: if you have negative with 0 scale, this will return rotation that is identity
	// since matrix loses that axes
	Quat Rotation = Quat(M);
	Rotation.Normalize();

	// set values back to output
	OutTransform.Scale3D = DesiredScale;
	OutTransform.MRotation = Rotation;

	// technically I could calculate this using Transform but then it does more quat multiplication 
	// instead of using Scale in matrix multiplication
	// it's a question of between RemoveScaling vs using Transform to move translation
	OutTransform.Translation = M.GetOrigin();
}

/** Returns Multiplied Transform of 2 Transforms **/
void Transform::Multiply(Transform* OutTransform, const Transform* A, const Transform* B)
{
	A->DiagnosticCheckNaN_All();
	B->DiagnosticCheckNaN_All();

	checkSlow(A->IsRotationNormalized());
	checkSlow(B->IsRotationNormalized());

	//	When Q = quaternion, S = single scalar scale, and T = translation
	//	QST(A) = Q(A), S(A), T(A), and QST(B) = Q(B), S(B), T(B)

	//	QST (AxB) 

	// QST(A) = Q(A)*S(A)*P*-Q(A) + T(A)
	// QST(AxB) = Q(B)*S(B)*QST(A)*-Q(B) + T(B)
	// QST(AxB) = Q(B)*S(B)*[Q(A)*S(A)*P*-Q(A) + T(A)]*-Q(B) + T(B)
	// QST(AxB) = Q(B)*S(B)*Q(A)*S(A)*P*-Q(A)*-Q(B) + Q(B)*S(B)*T(A)*-Q(B) + T(B)
	// QST(AxB) = [Q(B)*Q(A)]*[S(B)*S(A)]*P*-[Q(B)*Q(A)] + Q(B)*S(B)*T(A)*-Q(B) + T(B)

	//	Q(AxB) = Q(B)*Q(A)
	//	S(AxB) = S(A)*S(B)
	//	T(AxB) = Q(B)*S(B)*T(A)*-Q(B) + T(B)

	if (AnyHasNegativeScale(A->Scale3D, B->Scale3D))
	{
		// @note, if you have 0 scale with negative, you're going to lose rotation as it can't convert back to quat
		MultiplyUsingMatrixWithScale(OutTransform, A, B);
	}
	else
	{
		OutTransform->MRotation = B->MRotation*A->MRotation;
		OutTransform->Scale3D = A->Scale3D*B->Scale3D;
		OutTransform->Translation = B->MRotation*(B->Scale3D*A->Translation) + B->Translation;
	}

	// we do not support matrix transform when non-uniform
	// that was removed at rev 21 with UE4
}
/**
* Apply Scale to this transform
*/
Transform Transform::GetScaled(float InScale) const
{
	Transform A(*this);
	A.Scale3D *= InScale;

	A.DiagnosticCheckNaN_Scale3D();

	return A;
}


/**
* Apply Scale to this transform
*/
Transform Transform::GetScaled(Vector InScale) const
{
	Transform A(*this);
	A.Scale3D *= InScale;

	A.DiagnosticCheckNaN_Scale3D();

	return A;
}


/** Transform homogenous Vector4, ignoring the scaling part of this transform **/
Vector4 Transform::TransformVector4NoScale(const Vector4& V) const
{
	DiagnosticCheckNaN_All();

	// if not, this won't work
	checkSlow(V.W == 0.f || V.W == 1.f);

	//Transform using QST is following
	//QST(P) = Q*S*P*-Q + T where Q = quaternion, S = scale, T = translation
	Vector4 Transform = Vector4(MRotation.RotateVector(Vector(V)), 0.f);
	if (V.W == 1.f)
	{
		Transform += Vector4(Translation, 1.f);
	}

	return Transform;
}


/** Transform Vector4 **/
Vector4 Transform::TransformVector4(const Vector4& V) const
{
	DiagnosticCheckNaN_All();

	// if not, this won't work
	checkSlow(V.W == 0.f || V.W == 1.f);

	//Transform using QST is following
	//QST(P) = Q*S*P*-Q + T where Q = quaternion, S = scale, T = translation

	Vector4 Transform = Vector4(MRotation.RotateVector(Scale3D*Vector(V)), 0.f);
	if (V.W == 1.f)
	{
		Transform += Vector4(Translation, 1.f);
	}

	return Transform;
}


Vector Transform::TransformPosition(const Vector& V) const
{
	DiagnosticCheckNaN_All();
	return MRotation.RotateVector(Scale3D*V) + Translation;
}


Vector Transform::TransformPositionNoScale(const Vector& V) const
{
	DiagnosticCheckNaN_All();
	return MRotation.RotateVector(V) + Translation;
}


Vector Transform::TransformVector(const Vector& V) const
{
	DiagnosticCheckNaN_All();
	return MRotation.RotateVector(Scale3D*V);
}


Vector Transform::TransformVectorNoScale(const Vector& V) const
{
	DiagnosticCheckNaN_All();
	return MRotation.RotateVector(V);
}


// do backward operation when inverse, translation -> rotation -> scale
Vector Transform::InverseTransformPosition(const Vector &V) const
{
	DiagnosticCheckNaN_All();
	return (MRotation.UnrotateVector(V - Translation)) * GetSafeScaleReciprocal(Scale3D);
}


// do backward operation when inverse, translation -> rotation
Vector Transform::InverseTransformPositionNoScale(const Vector &V) const
{
	DiagnosticCheckNaN_All();
	return (MRotation.UnrotateVector(V - Translation));
}


// do backward operation when inverse, translation -> rotation -> scale
Vector Transform::InverseTransformVector(const Vector &V) const
{
	DiagnosticCheckNaN_All();
	return (MRotation.UnrotateVector(V)) * GetSafeScaleReciprocal(Scale3D);
}


// do backward operation when inverse, translation -> rotation
Vector Transform::InverseTransformVectorNoScale(const Vector &V) const
{
	DiagnosticCheckNaN_All();
	return (MRotation.UnrotateVector(V));
}

Quat Transform::TransformRotation(const Quat& Q) const
{
	return GetRotation() * Q;
}

Quat Transform::InverseTransformRotation(const Quat& Q) const
{
	return GetRotation().Inverse() * Q;
}

Transform Transform::operator*(const Transform& Other) const
{
	Transform Output;
	Multiply(&Output, this, &Other);
	return Output;
}


void Transform::operator*=(const Transform& Other)
{
	Multiply(this, this, &Other);
}


Transform Transform::operator*(const Quat& Other) const
{
	Transform Output, OtherTransform(Other, Vector::ZeroVector, Vector::OneVector);
	Multiply(&Output, this, &OtherTransform);
	return Output;
}


void Transform::operator*=(const Quat& Other)
{
	Transform OtherTransform(Other, Vector::ZeroVector, Vector::OneVector);
	Multiply(this, this, &OtherTransform);
}


// x = 0, y = 1, z = 2
Vector Transform::GetScaledAxis(Axis InAxis) const
{
	if (InAxis == Axis::X)
	{
		return TransformVector(Vector(1.f, 0.f, 0.f));
	}
	else if (InAxis == Axis::Y)
	{
		return TransformVector(Vector(0.f, 1.f, 0.f));
	}

	return TransformVector(Vector(0.f, 0.f, 1.f));
}


// x = 0, y = 1, z = 2
Vector Transform::GetUnitAxis(Axis InAxis) const
{
	if (InAxis == Axis::X)
	{
		return TransformVectorNoScale(Vector(1.f, 0.f, 0.f));
	}
	else if (InAxis == Axis::Y)
	{
		return TransformVectorNoScale(Vector(0.f, 1.f, 0.f));
	}

	return TransformVectorNoScale(Vector(0.f, 0.f, 1.f));
}


void Transform::Mirror(Axis MirrorAxis, Axis FlipAxis)
{
	// We do convert to Matrix for mirroring. 
	Matrix M = ToMatrixWithScale();
	M.Mirror(MirrorAxis, FlipAxis);
	SetFromMatrix(M);
}




