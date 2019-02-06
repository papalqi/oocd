#include "Transform.h"
#include "ScalarRegister.h"
#include "VectorRegister.h"
oocd::Transform::Transform()
{
	// Rotation = {0,0,0,1)
	MRotation = VectorSet_W1(VectorZero());
	// Translation = {0,0,0,0)
	Translation = VectorZero();
	// Scale3D = {1,1,1,0);
	Scale3D = VectorSet_W0(VectorOne());
}

oocd::Transform::Transform(const VectorRegister & InRotation, const VectorRegister& InTranslation, const VectorRegister& InScale3D) : MRotation(InRotation),
Translation(InTranslation),
Scale3D(InScale3D)
{
}

oocd::Transform::Transform(const Vector& InTranslation)
{
	// Rotation = {0,0,0,1) quaternion identity
	MRotation = VectorSet_W1(VectorZero());
	//Translation = InTranslation;
	Translation = MakeVectorRegister(InTranslation.X, InTranslation.Y, InTranslation.Z, 0.0f);
	// Scale3D = {1,1,1,0);
	Scale3D = VectorSet_W0(VectorOne());
}

oocd::Transform::Transform(const Quat& InRotation)
{
	// Rotation = InRotation
	MRotation = VectorLoadAligned(&InRotation.X);
	// Translation = {0,0,0,0)
	Translation = VectorZero();
	// Scale3D = {1,1,1,0);
	Scale3D = VectorSet_W0(VectorOne());
}

oocd::Transform::Transform(const oocd::Rotator& InRotation)
{
	Quat InQuatRotation = InRotation.Quaternion();
	// Rotation = InRotation
	MRotation = VectorLoadAligned(&InQuatRotation.X);
	// Translation = {0,0,0,0)
	Translation = VectorZero();
	// Scale3D = {1,1,1,0);
	Scale3D = VectorSet_W0(VectorOne());
}

oocd::Transform::Transform(const Quat& InRotation, const Vector& InTranslation, const Vector& InScale3D /*= Vector::OneVector*/)
{
	// Rotation = InRotation
	MRotation = VectorLoadAligned(&InRotation.X);
	// Translation = InTranslation
	Translation = MakeVectorRegister(InTranslation.X, InTranslation.Y, InTranslation.Z, 0.0f);
	// Scale3D = InScale3D
	Scale3D = MakeVectorRegister(InScale3D.X, InScale3D.Y, InScale3D.Z, 0.0f);
}

oocd::Transform::Transform(const oocd::Rotator& InRotation, const Vector& InTranslation, const Vector& InScale3D /*= Vector::OneVector*/)
{
	Quat InQuatRotation = InRotation.Quaternion();
	// Rotation = InRotation
	MRotation = VectorLoadAligned(&InQuatRotation.X);
	// Translation = InTranslation
	Translation = MakeVectorRegister(InTranslation.X, InTranslation.Y, InTranslation.Z, 0.0f);
	// Scale3D = InScale3D
	Scale3D = MakeVectorRegister(InScale3D.X, InScale3D.Y, InScale3D.Z, 0.0f);
}

oocd::Transform::Transform(const Transform& InTransform) :
	MRotation(InTransform.MRotation),
	Translation(InTransform.Translation),
	Scale3D(InTransform.Scale3D)
{
}

oocd::Transform::Transform(const Matrix& InMatrix)
{
	SetFromMatrix(InMatrix);
}

oocd::Transform::Transform(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InTranslation)
{
	SetFromMatrix(Matrix(InX, InY, InZ, InTranslation));
}

void oocd::Transform::DebugPrint() const
{
}



oocd::Matrix oocd::Transform::ToMatrixWithScale() const
{
	Matrix OutMatrix;
	VectorRegister DiagonalsXYZ;
	VectorRegister Adds;
	VectorRegister Subtracts;

	ToMatrixInternal(DiagonalsXYZ, Adds, Subtracts);
	const VectorRegister DiagonalsXYZ_W0 = VectorSet_W0(DiagonalsXYZ);

	// OutMatrix.M[0][0] = (1.0f - (yy2 + zz2)) * Scale.X;    // Diagonal.X
	// OutMatrix.M[0][1] = (xy2 + wz2) * Scale.X;             // Adds.X
	// OutMatrix.M[0][2] = (xz2 - wy2) * Scale.X;             // Subtracts.Z
	// OutMatrix.M[0][3] = 0.0f;                              // DiagonalsXYZ_W0.W
	const VectorRegister AddX_DC_DiagX_DC = VectorShuffle(Adds, DiagonalsXYZ_W0, 0, 0, 0, 0);
	const VectorRegister SubZ_DC_DiagW_DC = VectorShuffle(Subtracts, DiagonalsXYZ_W0, 2, 0, 3, 0);
	const VectorRegister Row0 = VectorShuffle(AddX_DC_DiagX_DC, SubZ_DC_DiagW_DC, 2, 0, 0, 2);

	// OutMatrix.M[1][0] = (xy2 - wz2) * Scale.Y;             // Subtracts.X
	// OutMatrix.M[1][1] = (1.0f - (xx2 + zz2)) * Scale.Y;    // Diagonal.Y
	// OutMatrix.M[1][2] = (yz2 + wx2) * Scale.Y;             // Adds.Y
	// OutMatrix.M[1][3] = 0.0f;                            // DiagonalsXYZ_W0.W
	const VectorRegister SubX_DC_DiagY_DC = VectorShuffle(Subtracts, DiagonalsXYZ_W0, 0, 0, 1, 0);
	const VectorRegister AddY_DC_DiagW_DC = VectorShuffle(Adds, DiagonalsXYZ_W0, 1, 0, 3, 0);
	const VectorRegister Row1 = VectorShuffle(SubX_DC_DiagY_DC, AddY_DC_DiagW_DC, 0, 2, 0, 2);

	// OutMatrix.M[2][0] = (xz2 + wy2) * Scale.Z;             // Adds.Z
	// OutMatrix.M[2][1] = (yz2 - wx2) * Scale.Z;             // Subtracts.Y
	// OutMatrix.M[2][2] = (1.0f - (xx2 + yy2)) * Scale.Z;    // Diagonals.Z
	// OutMatrix.M[2][3] = 0.0f;                              // DiagonalsXYZ_W0.W
	const VectorRegister AddZ_DC_SubY_DC = VectorShuffle(Adds, Subtracts, 2, 0, 1, 0);
	const VectorRegister Row2 = VectorShuffle(AddZ_DC_SubY_DC, DiagonalsXYZ_W0, 0, 2, 2, 3);

	VectorStoreAligned(Row0, &(OutMatrix.M[0][0]));
	VectorStoreAligned(Row1, &(OutMatrix.M[1][0]));
	VectorStoreAligned(Row2, &(OutMatrix.M[2][0]));

	// OutMatrix.M[3][0] = Translation.X;
	// OutMatrix.M[3][1] = Translation.Y;
	// OutMatrix.M[3][2] = Translation.Z;
	// OutMatrix.M[3][3] = 1.0f;
	const VectorRegister Row3 = VectorSet_W1(Translation);
	VectorStoreAligned(Row3, &(OutMatrix.M[3][0]));

	return OutMatrix;
}

oocd::Matrix oocd::Transform::ToInverseMatrixWithScale() const
{
	// todo: optimize
	return ToMatrixWithScale().Inverse();
}

oocd::Transform oocd::Transform::Inverse() const
{
	// Replacement of Inverse of Matrix
	if (VectorAnyGreaterThan(VectorAbs(Scale3D), GlobalVectorConstants::SmallNumber))
	{
		return InverseFast();
	}
	else
	{
		return Transform::Identity;
	}
}

oocd::Matrix oocd::Transform::ToMatrixNoScale() const
{
	Matrix OutMatrix;
	VectorRegister DiagonalsXYZ;
	VectorRegister Adds;
	VectorRegister Subtracts;

	ToMatrixInternalNoScale(DiagonalsXYZ, Adds, Subtracts);
	const VectorRegister DiagonalsXYZ_W0 = VectorSet_W0(DiagonalsXYZ);

	// OutMatrix.M[0][0] = (1.0f - (yy2 + zz2));			// Diagonal.X
	// OutMatrix.M[0][1] = (xy2 + wz2);						// Adds.X
	// OutMatrix.M[0][2] = (xz2 - wy2);						// Subtracts.Z
	// OutMatrix.M[0][3] = 0.0f;                            // DiagonalsXYZ_W0.W
	const VectorRegister AddX_DC_DiagX_DC = VectorShuffle(Adds, DiagonalsXYZ_W0, 0, 0, 0, 0);
	const VectorRegister SubZ_DC_DiagW_DC = VectorShuffle(Subtracts, DiagonalsXYZ_W0, 2, 0, 3, 0);
	const VectorRegister Row0 = VectorShuffle(AddX_DC_DiagX_DC, SubZ_DC_DiagW_DC, 2, 0, 0, 2);

	// OutMatrix.M[1][0] = (xy2 - wz2);			            // Subtracts.X
	// OutMatrix.M[1][1] = (1.0f - (xx2 + zz2));		    // Diagonal.Y
	// OutMatrix.M[1][2] = (yz2 + wx2);						// Adds.Y
	// OutMatrix.M[1][3] = 0.0f;                            // DiagonalsXYZ_W0.W
	const VectorRegister SubX_DC_DiagY_DC = VectorShuffle(Subtracts, DiagonalsXYZ_W0, 0, 0, 1, 0);
	const VectorRegister AddY_DC_DiagW_DC = VectorShuffle(Adds, DiagonalsXYZ_W0, 1, 0, 3, 0);
	const VectorRegister Row1 = VectorShuffle(SubX_DC_DiagY_DC, AddY_DC_DiagW_DC, 0, 2, 0, 2);

	// OutMatrix.M[2][0] = (xz2 + wy2);						// Adds.Z
	// OutMatrix.M[2][1] = (yz2 - wx2);						// Subtracts.Y
	// OutMatrix.M[2][2] = (1.0f - (xx2 + yy2));		    // Diagonals.Z
	// OutMatrix.M[2][3] = 0.0f;                            // DiagonalsXYZ_W0.W
	const VectorRegister AddZ_DC_SubY_DC = VectorShuffle(Adds, Subtracts, 2, 0, 1, 0);
	const VectorRegister Row2 = VectorShuffle(AddZ_DC_SubY_DC, DiagonalsXYZ_W0, 0, 2, 2, 3);

	VectorStoreAligned(Row0, &(OutMatrix.M[0][0]));
	VectorStoreAligned(Row1, &(OutMatrix.M[1][0]));
	VectorStoreAligned(Row2, &(OutMatrix.M[2][0]));

	// OutMatrix.M[3][0] = Translation.X;
	// OutMatrix.M[3][1] = Translation.Y;
	// OutMatrix.M[3][2] = Translation.Z;
	// OutMatrix.M[3][3] = 1.0f;
	const VectorRegister Row3 = VectorSet_W1(Translation);
	VectorStoreAligned(Row3, &(OutMatrix.M[3][0]));

	return OutMatrix;
}

void oocd::Transform::Blend(const Transform& Atom1, const Transform& Atom2, float Alpha)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) && WITH_EDITORONLY_DATA
	// Check that all bone atoms coming from animation are normalized
	check(Atom1.IsRotationNormalized());
	check(Atom2.IsRotationNormalized());
#endif

	if (Math::Abs(Alpha) <= ZERO_ANIMWEIGHT_THRESH)
	{
		// if blend is all the way for child1, then just copy its bone atoms
		(*this) = Atom1;
	}
	else if (Math::Abs(Alpha - 1.0f) <= ZERO_ANIMWEIGHT_THRESH)
	{
		// if blend is all the way for child2, then just copy its bone atoms
		(*this) = Atom2;
	}
	else
	{
		// Simple linear interpolation for translation and scale.
		ScalarRegister BlendWeight = ScalarRegister(Alpha);

		Translation = Math::Lerp(Atom1.Translation, Atom2.Translation, BlendWeight.Value);
		Scale3D = Math::Lerp(Atom1.Scale3D, Atom2.Scale3D, BlendWeight.Value);

		VectorRegister VRotation = VectorLerpQuat(Atom1.MRotation, Atom2.MRotation, BlendWeight.Value);

		// ..and renormalize
		MRotation = VectorNormalizeQuaternion(VRotation);

		// MR
	}
}

void oocd::Transform::BlendWith(const Transform& OtherAtom, float Alpha)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) && WITH_EDITORONLY_DATA
	// Check that all bone atoms coming from animation are normalized
	check(IsRotationNormalized());
	check(OtherAtom.IsRotationNormalized());
#endif

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
			ScalarRegister BlendWeight = ScalarRegister(Alpha);
			Translation = Math::Lerp(Translation, OtherAtom.Translation, BlendWeight.Value);

			Scale3D = Math::Lerp(Scale3D, OtherAtom.Scale3D, BlendWeight.Value);

			VectorRegister VRotation = VectorLerpQuat(MRotation, OtherAtom.MRotation, BlendWeight.Value);

			// ..and renormalize
			MRotation = VectorNormalizeQuaternion(VRotation);
		}
	}
}

bool oocd::Transform::Private_AnyHasNegativeScale(const VectorRegister& InScale3D, const VectorRegister& InOtherScale3D)
{
	return !!VectorAnyLesserThan(VectorMin(InScale3D, InOtherScale3D), GlobalVectorConstants::FloatZero);
}

bool oocd::Transform::Private_RotationEquals(const VectorRegister& InRotation, const ScalarRegister& Tolerance /*= ScalarRegister(GlobalVectorConstants::KindaSmallNumber)*/) const
{
	// !( (Math::Abs(X-Q.X) > Tolerance) || (Math::Abs(Y-Q.Y) > Tolerance) || (Math::Abs(Z-Q.Z) > Tolerance) || (Math::Abs(W-Q.W) > Tolerance) )
	const VectorRegister RotationSub = VectorAbs(VectorSubtract(MRotation, InRotation));
	// !( (Math::Abs(X+Q.X) > Tolerance) || (Math::Abs(Y+Q.Y) > Tolerance) || (Math::Abs(Z+Q.Z) > Tolerance) || (Math::Abs(W+Q.W) > Tolerance) )
	const VectorRegister RotationAdd = VectorAbs(VectorAdd(MRotation, InRotation));
	return !VectorAnyGreaterThan(RotationSub, Tolerance.Value) || !VectorAnyGreaterThan(RotationAdd, Tolerance.Value);
}

bool oocd::Transform::Private_TranslationEquals(const VectorRegister& InTranslation, const ScalarRegister& Tolerance /*= ScalarRegister(GlobalVectorConstants::KindaSmallNumber)*/) const
{
	// !( (Math::Abs(X-V.X) > Tolerance) || (Math::Abs(Y-V.Y) > Tolerance) || (Math::Abs(Z-V.Z) > Tolerance) )
	const VectorRegister TranslationDiff = VectorAbs(VectorSubtract(Translation, InTranslation));
	return !VectorAnyGreaterThan(TranslationDiff, Tolerance.Value);
}

bool oocd::Transform::Private_Scale3DEquals(const VectorRegister& InScale3D, const ScalarRegister& Tolerance /*= ScalarRegister(GlobalVectorConstants::KindaSmallNumber)*/) const
{
	// !( (Math::Abs(X-V.X) > Tolerance) || (Math::Abs(Y-V.Y) > Tolerance) || (Math::Abs(Z-V.Z) > Tolerance) )
	const VectorRegister ScaleDiff = VectorAbs(VectorSubtract(Scale3D, InScale3D));
	return !VectorAnyGreaterThan(ScaleDiff, Tolerance.Value);
}

bool oocd::Transform::AreRotationsEqual(const Transform& A, const Transform& B, float Tolerance /*= KINDA_SMALL_NUMBER*/)
{
	return A.Private_RotationEquals(B.MRotation, ScalarRegister(Tolerance));
}

bool oocd::Transform::AreTranslationsEqual(const Transform& A, const Transform& B, float Tolerance /*= KINDA_SMALL_NUMBER*/)
{
	return A.Private_TranslationEquals(B.Translation, ScalarRegister(Tolerance));
}

bool oocd::Transform::AreScale3DsEqual(const Transform& A, const Transform& B, float Tolerance /*= KINDA_SMALL_NUMBER*/)
{
	return A.Private_Scale3DEquals(B.Scale3D, ScalarRegister(Tolerance));
}

bool oocd::Transform::RotationEquals(const Transform& Other, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	return AreRotationsEqual(*this, Other, Tolerance);
}

bool oocd::Transform::TranslationEquals(const Transform& Other, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	return AreTranslationsEqual(*this, Other, Tolerance);
}

bool oocd::Transform::Scale3DEquals(const Transform& Other, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	return AreScale3DsEqual(*this, Other, Tolerance);
}

bool oocd::Transform::Equals(const Transform& Other, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	const ScalarRegister ToleranceRegister(Tolerance);
	return Private_TranslationEquals(Other.Translation, ToleranceRegister) && Private_RotationEquals(Other.MRotation, ToleranceRegister) && Private_Scale3DEquals(Other.Scale3D, ToleranceRegister);
}

bool oocd::Transform::EqualsNoScale(const Transform& Other, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	const ScalarRegister ToleranceRegister(Tolerance);
	return Private_TranslationEquals(Other.Translation, ToleranceRegister) && Private_RotationEquals(Other.MRotation, ToleranceRegister);
}

void oocd::Transform::SetComponents(const Quat& InRotation, const Vector& InTranslation, const Vector& InScale3D)
{
	MRotation = VectorLoadAligned(&InRotation);
	Translation = VectorLoadFloat3_W0(&InTranslation);
	Scale3D = VectorLoadFloat3_W0(&InScale3D);
}

void oocd::Transform::SetIdentity()
{
	// Rotation = {0,0,0,1)
	MRotation = VectorSet_W1(VectorZero());
	// Translation = {0,0,0,0)
	Translation = VectorZero();
	// Scale3D = {1,1,1,0);
	Scale3D = VectorSet_W0(VectorOne());
}

void oocd::Transform::MultiplyScale3D(const Vector& Scale3DMultiplier)
{
	Scale3D = VectorMultiply(Scale3D, VectorLoadFloat3_W0(&Scale3DMultiplier));
}

void oocd::Transform::SetTranslation(const Vector& NewTranslation)
{
	Translation = VectorLoadFloat3_W0(&NewTranslation);
}

void oocd::Transform::CopyTranslation(const Transform& Other)
{
	Translation = Other.Translation;
}

void oocd::Transform::ConcatenateRotation(const Quat& DeltaRotation)
{
	MRotation = VectorQuaternionMultiply2(MRotation, VectorLoadAligned(&DeltaRotation));
}

void oocd::Transform::AddToTranslation(const Vector& DeltaTranslation)
{
	Translation = VectorAdd(Translation, VectorLoadFloat3_W0(&DeltaTranslation));
}

oocd::Vector oocd::Transform::AddTranslations(const Transform& A, const Transform& B)
{
	Vector Result;
	VectorStoreFloat3(VectorAdd(A.Translation, B.Translation), &Result);
	return Result;
}

oocd::Vector oocd::Transform::SubtractTranslations(const Transform& A, const Transform& B)
{
	Vector Result;
	VectorStoreFloat3(VectorSubtract(A.Translation, B.Translation), &Result);
	return Result;
}

void oocd::Transform::SetRotation(const Quat& NewRotation)
{
	MRotation = VectorLoadAligned(&NewRotation);
}

void oocd::Transform::CopyRotation(const Transform& Other)
{
	MRotation = Other.MRotation;
}

void oocd::Transform::SetScale3D(const Vector& NewScale3D)
{
	Scale3D = VectorLoadFloat3_W0(&NewScale3D);
}

void oocd::Transform::CopyScale3D(const Transform& Other)
{
	Scale3D = Other.Scale3D;
}

void oocd::Transform::SetTranslationAndScale3D(const Vector& NewTranslation, const Vector& NewScale3D)
{
	Translation = VectorLoadFloat3_W0(&NewTranslation);
	Scale3D = VectorLoadFloat3_W0(&NewScale3D);


}

void oocd::Transform::Accumulate(const Transform& Atom, const ScalarRegister& BlendWeight)
{
	// SourceAtom = Atom * BlendWeight;
	const VectorRegister BlendedRotation = VectorMultiply(Atom.MRotation, BlendWeight.Value);
	const VectorRegister BlendedTranslation = VectorMultiply(Atom.Translation, BlendWeight.Value);
	const VectorRegister BlendedScale = VectorMultiply(Atom.Scale3D, BlendWeight.Value);

	const VectorRegister RotationW = VectorReplicate(BlendedRotation, 3);

	// Add ref pose relative animation to base animation, only if rotation is significant.
	// if( Square(SourceAtom.Rotation.W) < 1.f - DELTA * DELTA )
	if (VectorAnyGreaterThan(GlobalVectorConstants::RotationSignificantThreshold, VectorMultiply(RotationW, RotationW)))
	{
		// Rotation = SourceAtom.Rotation * Rotation;
		MRotation = VectorQuaternionMultiply2(BlendedRotation, MRotation);
	}

	// Translation += SourceAtom.Translation;
	// Scale *= SourceAtom.Scale;
	Translation = VectorAdd(Translation, BlendedTranslation);
	Scale3D = VectorMultiply(Scale3D, BlendedScale);
}

void oocd::Transform::Accumulate(const Transform& SourceAtom)
{
	const VectorRegister BlendedRotation = SourceAtom.MRotation;
	const VectorRegister RotationW = VectorReplicate(BlendedRotation, 3);

	// if( Square(SourceAtom.Rotation.W) < 1.f - DELTA * DELTA )
	if (VectorAnyGreaterThan(GlobalVectorConstants::RotationSignificantThreshold, VectorMultiply(RotationW, RotationW)))
	{
		// Rotation = SourceAtom.Rotation * Rotation;
		MRotation = VectorQuaternionMultiply2(BlendedRotation, MRotation);
	}

	// Translation += SourceAtom.Translation;
	// Scale *= SourceAtom.Scale;
	Translation = VectorAdd(Translation, SourceAtom.Translation);
	Scale3D = VectorMultiply(Scale3D, SourceAtom.Scale3D);

	checkSlow(IsRotationNormalized());
}

void oocd::Transform::AccumulateWithShortestRotation(const Transform& DeltaAtom, const ScalarRegister& BlendWeight)
{
	const VectorRegister BlendedRotation = VectorMultiply(DeltaAtom.MRotation, BlendWeight.Value);

	MRotation = VectorAccumulateQuaternionShortestPath(MRotation, BlendedRotation);

	Translation = VectorMultiplyAdd(DeltaAtom.Translation, BlendWeight, Translation);
	Scale3D = VectorMultiplyAdd(DeltaAtom.Scale3D, BlendWeight, Scale3D);
}

void oocd::Transform::AccumulateWithAdditiveScale(const Transform& Atom, const ScalarRegister& BlendWeight)
{
	const VectorRegister DefaultScale = MakeVectorRegister(1.f, 1.f, 1.f, 0.f);

	// SourceAtom = Atom * BlendWeight;
	const VectorRegister BlendedRotation = VectorMultiply(Atom.MRotation, BlendWeight.Value);
	const VectorRegister BlendedScale = VectorMultiply(Atom.Scale3D, BlendWeight.Value);
	const VectorRegister BlendedTranslation = VectorMultiply(Atom.Translation, BlendWeight.Value);

	const VectorRegister RotationW = VectorReplicate(BlendedRotation, 3);

	// Add ref pose relative animation to base animation, only if rotation is significant.
	// if( Square(SourceAtom.Rotation.W) < 1.f - DELTA * DELTA )
	if (VectorAnyGreaterThan(GlobalVectorConstants::RotationSignificantThreshold, VectorMultiply(RotationW, RotationW)))
	{
		// Rotation = SourceAtom.Rotation * Rotation;
		MRotation = VectorQuaternionMultiply2(BlendedRotation, MRotation);
	}

	// Translation += SourceAtom.Translation;
	// Scale *= SourceAtom.Scale;
	Translation = VectorAdd(Translation, BlendedTranslation);
	Scale3D = VectorMultiply(Scale3D, VectorAdd(DefaultScale, BlendedScale));
}

void oocd::Transform::LerpTranslationScale3D(const Transform& SourceAtom1, const Transform& SourceAtom2, const ScalarRegister& Alpha)
{
	Translation = Math::Lerp(SourceAtom1.Translation, SourceAtom2.Translation, Alpha.Value);
	Scale3D = Math::Lerp(SourceAtom1.Scale3D, SourceAtom2.Scale3D, Alpha.Value);


}

bool oocd::Transform::IsRotationNormalized() const
{
	const VectorRegister TestValue = VectorAbs(VectorSubtract(VectorOne(), VectorDot4(MRotation, MRotation)));
	return !VectorAnyGreaterThan(TestValue, GlobalVectorConstants::ThreshQuatNormalized);
}

void oocd::Transform::BlendFromIdentityAndAccumulate(Transform& FinalAtom, Transform& SourceAtom, const ScalarRegister& BlendWeight)
{
	const VectorRegister Const0001 = GlobalVectorConstants::Float0001;
	const VectorRegister ConstNegative0001 = VectorSubtract(VectorZero(), Const0001);
	const VectorRegister VOneMinusAlpha = VectorSubtract(VectorOne(), BlendWeight.Value);
	const VectorRegister DefaultScale = MakeVectorRegister(1.f, 1.f, 1.f, 0.f);

	// Blend rotation
	//     To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
	//     const float Bias = (|A.B| >= 0 ? 1 : -1)
	//     BlendedAtom.Rotation = (B * Alpha) + (A * (Bias * (1.f - Alpha)));
	//     BlendedAtom.Rotation.QuaternionNormalize();
	//  Note: A = (0,0,0,1), which simplifies things a lot; only care about sign of B.W now, instead of doing a dot product
	const VectorRegister RotationB = SourceAtom.MRotation;

	const VectorRegister QuatRotationDirMask = VectorCompareGE(RotationB, VectorZero());
	const VectorRegister BiasTimesA = VectorSelect(QuatRotationDirMask, Const0001, ConstNegative0001);
	const VectorRegister RotateBTimesWeight = VectorMultiply(RotationB, BlendWeight.Value);
	const VectorRegister UnnormalizedRotation = VectorMultiplyAdd(BiasTimesA, VOneMinusAlpha, RotateBTimesWeight);

	// Normalize blended rotation ( result = (Q.Q >= 1e-8) ? (Q / |Q|) : (0,0,0,1) )
	const VectorRegister BlendedRotation = VectorNormalizeSafe(UnnormalizedRotation, Const0001);

	// FinalAtom.Rotation = BlendedAtom.Rotation * FinalAtom.Rotation;
	FinalAtom.MRotation = VectorQuaternionMultiply2(BlendedRotation, FinalAtom.MRotation);

	// Blend translation and scale
	//    BlendedAtom.Translation = Lerp(Zero, SourceAtom.Translation, Alpha);
	//    BlendedAtom.Scale = Lerp(0, SourceAtom.Scale, Alpha);
	const VectorRegister BlendedTranslation = Math::Lerp(VectorZero(), SourceAtom.Translation, BlendWeight.Value);
	const VectorRegister BlendedScale3D = Math::Lerp(VectorZero(), SourceAtom.Scale3D, BlendWeight.Value);

	// Apply translation and scale to final atom
	//     FinalAtom.Translation += BlendedAtom.Translation
	//     FinalAtom.Scale *= BlendedAtom.Scale
	FinalAtom.Translation = VectorAdd(FinalAtom.Translation, BlendedTranslation);
	FinalAtom.Scale3D = VectorMultiply(FinalAtom.Scale3D, VectorAdd(DefaultScale, BlendedScale3D));
	checkSlow(FinalAtom.IsRotationNormalized());
}

oocd::Quat oocd::Transform::GetRotation() const
{

	Quat OutRotation;
	VectorStoreAligned(MRotation, &OutRotation);
	return OutRotation;
}

oocd::Vector oocd::Transform::GetTranslation() const
{

	Vector OutTranslation;
	VectorStoreFloat3(Translation, &OutTranslation);
	return OutTranslation;
}

oocd::Vector oocd::Transform::GetScale3D() const
{

	Vector OutScale3D;
	VectorStoreFloat3(Scale3D, &OutScale3D);
	return OutScale3D;
}

void oocd::Transform::CopyRotationPart(const Transform& SrcBA)
{
	MRotation = SrcBA.MRotation;
	Scale3D = SrcBA.Scale3D;

}

void oocd::Transform::CopyTranslationAndScale3D(const Transform& SrcBA)
{
	Translation = SrcBA.Translation;
	Scale3D = SrcBA.Scale3D;


}

void oocd::Transform::SetFromMatrix(const Matrix& InMatrix)
{
	Matrix M = InMatrix;

	// Get the 3D scale from the matrix
	Vector InScale = M.ExtractScaling();
	Scale3D = VectorLoadFloat3_W0(&InScale);

	// If there is negative scaling going on, we handle that here
	if (InMatrix.Determinant() < 0.f)
	{
		// Assume it is along X and modify transform accordingly.
		// It doesn't actually matter which axis we choose, the 'appearance' will be the same
		Scale3D = VectorMultiply(Scale3D, GlobalVectorConstants::FloatMinus1_111);
		M.SetAxis(0, -M.GetScaledAxis(Axis::X));
	}

	Quat InRotation = Quat(M);
	MRotation = VectorLoadAligned(&InRotation);
	Vector InTranslation = InMatrix.GetOrigin();
	Translation = VectorLoadFloat3_W0(&InTranslation);

	// Normalize rotation
	MRotation = VectorNormalizeQuaternion(MRotation);
}

void oocd::Transform::ToMatrixInternal(VectorRegister& OutDiagonals, VectorRegister& OutAdds, VectorRegister& OutSubtracts) const
{


	const VectorRegister RotationX2Y2Z2 = VectorAdd(MRotation, MRotation);	// x2, y2, z2
	const VectorRegister RotationXX2YY2ZZ2 = VectorMultiply(RotationX2Y2Z2, MRotation);	// xx2, yy2, zz2

	// The diagonal terms of the rotation matrix are:
	//   (1 - (yy2 + zz2)) * scale
	//   (1 - (xx2 + zz2)) * scale
	//   (1 - (xx2 + yy2)) * scale
	const VectorRegister yy2_xx2_xx2 = VectorSwizzle(RotationXX2YY2ZZ2, 1, 0, 0, 0);
	const VectorRegister zz2_zz2_yy2 = VectorSwizzle(RotationXX2YY2ZZ2, 2, 2, 1, 0);
	const VectorRegister DiagonalSum = VectorAdd(yy2_xx2_xx2, zz2_zz2_yy2);
	const VectorRegister Diagonals = VectorSubtract(VectorOne(), DiagonalSum);
	OutDiagonals = VectorMultiply(Diagonals, Scale3D);

	// Grouping the non-diagonal elements in the rotation block by operations:
	//    ((x*y2,y*z2,x*z2) + (w*z2,w*x2,w*y2)) * scale.xyz and
	//    ((x*y2,y*z2,x*z2) - (w*z2,w*x2,w*y2)) * scale.yxz
	// Rearranging so the LHS and RHS are in the same order as for +
	//    ((x*y2,y*z2,x*z2) - (w*z2,w*x2,w*y2)) * scale.yxz

	// RotBase = x*y2, y*z2, x*z2
	// RotOffset = w*z2, w*x2, w*y2
	const VectorRegister x_y_x = VectorSwizzle(MRotation, 0, 1, 0, 0);
	const VectorRegister y2_z2_z2 = VectorSwizzle(RotationX2Y2Z2, 1, 2, 2, 0);
	const VectorRegister RotBase = VectorMultiply(x_y_x, y2_z2_z2);

	const VectorRegister w_w_w = VectorReplicate(MRotation, 3);
	const VectorRegister z2_x2_y2 = VectorSwizzle(RotationX2Y2Z2, 2, 0, 1, 0);
	const VectorRegister RotOffset = VectorMultiply(w_w_w, z2_x2_y2);

	// Adds = (RotBase + RotOffset)*Scale3D :  (x*y2 + w*z2) * Scale3D.X , (y*z2 + w*x2) * Scale3D.Y, (x*z2 + w*y2) * Scale3D.Z
	// Subtracts = (RotBase - RotOffset)*Scale3DYZX :  (x*y2 - w*z2) * Scale3D.Y , (y*z2 - w*x2) * Scale3D.Z, (x*z2 - w*y2) * Scale3D.X
	const VectorRegister Adds = VectorAdd(RotBase, RotOffset);
	OutAdds = VectorMultiply(Adds, Scale3D);
	const VectorRegister Scale3DYZXW = VectorSwizzle(Scale3D, 1, 2, 0, 3);
	const VectorRegister Subtracts = VectorSubtract(RotBase, RotOffset);
	OutSubtracts = VectorMultiply(Subtracts, Scale3DYZXW);
}

void oocd::Transform::ToMatrixInternalNoScale(VectorRegister& OutDiagonals, VectorRegister& OutAdds, VectorRegister& OutSubtracts) const
{

	const VectorRegister RotationX2Y2Z2 = VectorAdd(MRotation, MRotation);	// x2, y2, z2
	const VectorRegister RotationXX2YY2ZZ2 = VectorMultiply(RotationX2Y2Z2, MRotation);	// xx2, yy2, zz2

	// The diagonal terms of the rotation matrix are:
	//   (1 - (yy2 + zz2))
	//   (1 - (xx2 + zz2))
	//   (1 - (xx2 + yy2))
	const VectorRegister yy2_xx2_xx2 = VectorSwizzle(RotationXX2YY2ZZ2, 1, 0, 0, 0);
	const VectorRegister zz2_zz2_yy2 = VectorSwizzle(RotationXX2YY2ZZ2, 2, 2, 1, 0);
	const VectorRegister DiagonalSum = VectorAdd(yy2_xx2_xx2, zz2_zz2_yy2);
	OutDiagonals = VectorSubtract(VectorOne(), DiagonalSum);

	// Grouping the non-diagonal elements in the rotation block by operations:
	//    ((x*y2,y*z2,x*z2) + (w*z2,w*x2,w*y2)) and
	//    ((x*y2,y*z2,x*z2) - (w*z2,w*x2,w*y2))
	// Rearranging so the LHS and RHS are in the same order as for +
	//    ((x*y2,y*z2,x*z2) - (w*z2,w*x2,w*y2))

	// RotBase = x*y2, y*z2, x*z2
	// RotOffset = w*z2, w*x2, w*y2
	const VectorRegister x_y_x = VectorSwizzle(MRotation, 0, 1, 0, 0);
	const VectorRegister y2_z2_z2 = VectorSwizzle(RotationX2Y2Z2, 1, 2, 2, 0);
	const VectorRegister RotBase = VectorMultiply(x_y_x, y2_z2_z2);

	const VectorRegister w_w_w = VectorReplicate(MRotation, 3);
	const VectorRegister z2_x2_y2 = VectorSwizzle(RotationX2Y2Z2, 2, 0, 1, 0);
	const VectorRegister RotOffset = VectorMultiply(w_w_w, z2_x2_y2);

	// Adds = (RotBase + RotOffset):  (x*y2 + w*z2) , (y*z2 + w*x2), (x*z2 + w*y2)
	// Subtracts = (RotBase - RotOffset) :  (x*y2 - w*z2) , (y*z2 - w*x2), (x*z2 - w*y2)
	OutAdds = VectorAdd(RotBase, RotOffset);
	OutSubtracts = VectorSubtract(RotBase, RotOffset);
}

VectorRegister oocd::Transform::GetSafeScaleReciprocal(const VectorRegister& InScale, const ScalarRegister& Tolerance /*= ScalarRegister(GlobalVectorConstants::SmallNumber)*/)
{
	// SafeReciprocalScale.X = (InScale.X == 0) ? 0.f : 1/InScale.X; // same for YZW
	VectorRegister SafeReciprocalScale;

	/// VectorRegister( 1.0f / InScale.x, 1.0f / InScale.y, 1.0f / InScale.z, 1.0f / InScale.w )
	const VectorRegister ReciprocalScale = VectorReciprocalAccurate(InScale);

	//VectorRegister( Vec1.x == Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
	const VectorRegister ScaleZeroMask = VectorCompareGE(Tolerance.Value, VectorAbs(InScale));

	//const VectorRegister ScaleZeroMask = VectorCompareEQ(InScale, VectorZero());

	// VectorRegister( for each bit i: Mask[i] ? Vec1[i] : Vec2[i] )
	SafeReciprocalScale = VectorSelect(ScaleZeroMask, VectorZero(), ReciprocalScale);

	return SafeReciprocalScale;
}

oocd::Vector oocd::Transform::GetLocation() const
{
	return GetTranslation();
}

oocd::Rotator oocd::Transform::Rotator() const
{
	Quat OutRotation;
	VectorStoreAligned(MRotation, &OutRotation);
	return OutRotation.Rotator();
}

float oocd::Transform::GetDeterminant() const
{
	//#todo - vectorized version of this
	Vector4 OutScale3D;
	VectorStoreAligned(Scale3D, &OutScale3D);
	return OutScale3D.X * OutScale3D.Y * OutScale3D.Z;
}

void oocd::Transform::SetLocation(const Vector& Origin)
{
	Translation = VectorLoadFloat3_W0(&Origin);
}

bool oocd::Transform::ContainsNaN() const
{
	if (VectorContainsNaNOrInfinite(MRotation))
	{
		return true;
	}
	if (VectorContainsNaNOrInfinite(Translation))
	{
		return true;
	}

	if (VectorContainsNaNOrInfinite(Scale3D))
	{
		return true;
	}
	return false;
}

oocd::Transform oocd::Transform::InverseFast() const
{
	// Inverse QST (A) = QST (~A)
	// Since A*~A = Identity,
	// A(P) = Q(A)*S(A)*P*-Q(A) + T(A)
	// ~A(A(P)) = Q(~A)*S(~A)*(Q(A)*S(A)*P*-Q(A) + T(A))*-Q(~A) + T(~A) = Identity
	// Q(~A)*Q(A)*S(~A)*S(A)*P*-Q(A)*-Q(~A) + Q(~A)*S(~A)*T(A)*-Q(~A) + T(~A) = Identity
	// [Q(~A)*Q(A)]*[S(~A)*S(A)]*P*-[Q(~A)*Q(A)] + [Q(~A)*S(~A)*T(A)*-Q(~A) + T(~A)] = I

	// Identity Q = (0, 0, 0, 1) = Q(~A)*Q(A)
	// Identity Scale = 1 = S(~A)*S(A)
	// Identity Translation = (0, 0, 0) = [Q(~A)*S(~A)*T(A)*-Q(~A) + T(~A)]

	//	Q(~A) = Q(~A)
	//	S(~A) = 1.f/S(A)
	//	T(~A) = - (Q(~A)*S(~A)*T(A)*Q(A))
	checkSlow(IsRotationNormalized());
	checkSlow(VectorAnyGreaterThan(VectorAbs(Scale3D), GlobalVectorConstants::SmallNumber));

	// Invert the scale
	const VectorRegister InvScale = VectorSet_W0(GetSafeScaleReciprocal(Scale3D, ScalarRegister(GlobalVectorConstants::SmallNumber)));

	// Invert the rotation
	const VectorRegister InvRotation = VectorQuaternionInverse(MRotation);

	// Invert the translation
	const VectorRegister ScaledTranslation = VectorMultiply(InvScale, Translation);
	const VectorRegister t2 = VectorQuaternionRotateVector(InvRotation, ScaledTranslation);
	const VectorRegister InvTranslation = VectorSet_W0(VectorNegate(t2));

	return Transform(InvRotation, InvTranslation, InvScale);
}

oocd::Transform& oocd::Transform::operator=(const Transform& Other)
{
	this->MRotation = Other.MRotation;
	this->Translation = Other.Translation;
	this->Scale3D = Other.Scale3D;

	return *this;
}