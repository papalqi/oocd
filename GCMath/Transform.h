#pragma once
#include "Vector.h"
#include "MathHelper.h"
#include "Matrix.h"
#include "ScalarRegister.h"
#include "VectorRegister.h"
#

namespace oocd
{
	class Transform
	{
		friend struct Z_Construct_UScriptStruct_Transform_Statics;

	protected:
		VectorRegister	MRotation;
		VectorRegister	Translation;
		VectorRegister Scale3D;
	public:

		static  const Transform Identity;

		Transform();

		explicit Transform(const Vector& InTranslation);

		explicit Transform(const Quat& InRotation);
		explicit Transform(const oocd::Rotator& InRotation);

	
		Transform(const Quat& InRotation, const Vector& InTranslation, const Vector& InScale3D = Vector::UnitVector);


		Transform(const VectorRegister	& InRotation, const VectorRegister& InTranslation, const VectorRegister& InScale3D);

		Transform(const oocd::Rotator& InRotation, const Vector& InTranslation, const Vector& InScale3D = Vector::OneVector);

		Transform(const Transform& InTransform);

		/**
		 * Constructor for converting a Matrix (including scale) into a Transform.
		 */
		explicit Transform(const Matrix& InMatrix);

		/** Constructor that takes basis axes and translation */
		Transform(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InTranslation);

		/**
		 * Does a debugf of the contents of this Transform.
		 */
		void DebugPrint() const;

	

		/**
		* Copy another Transform into this one
		*/
		Transform& operator=(const Transform& Other);

		Matrix ToMatrixWithScale() const;

		/**
		* Convert this Transform to matrix with scaling and compute the inverse of that.
		*/
		Matrix ToInverseMatrixWithScale() const;

		/**
		* Convert this Transform to inverse.
		*/
		Transform Inverse() const;

		/**
		* Convert this Transform to a transformation matrix, ignoring its scaling
		*/
		Matrix ToMatrixNoScale() const;

		/** Set this transform to the weighted blend of the supplied two transforms. */
		void Blend(const Transform& Atom1, const Transform& Atom2, float Alpha);

		/** Set this Transform to the weighted blend of it and the supplied Transform. */
		void BlendWith(const Transform& OtherAtom, float Alpha);

		/**
		 * Quaternion addition is wrong here. This is just a special case for linear interpolation.
		 * Use only within blends!!
		 * Rotation part is NOT normalized!!
		 */
		Transform operator+(const Transform& Atom) const
		{
			return Transform(VectorAdd(MRotation, Atom.MRotation), VectorAdd(Translation, Atom.Translation), VectorAdd(Scale3D, Atom.Scale3D));
		}

		Transform& operator+=(const Transform& Atom)
		{
			Translation = VectorAdd(Translation, Atom.Translation);
			MRotation = VectorAdd(MRotation, Atom.MRotation);
			Scale3D = VectorAdd(Scale3D, Atom.Scale3D);

			return *this;
		}

		Transform operator*(const ScalarRegister& Mult) const
		{
			return Transform(VectorMultiply(MRotation, Mult), VectorMultiply(Translation, Mult), VectorMultiply(Scale3D, Mult));
		}

		Transform& operator*=(const ScalarRegister& Mult)
		{
			Translation = VectorMultiply(Translation, Mult);
			MRotation = VectorMultiply(MRotation, Mult);
			Scale3D = VectorMultiply(Scale3D, Mult);

			return *this;
		}

		Transform		operator*(const Transform& Other) const;
		void			operator*=(const Transform& Other);
		Transform		operator*(const Quat& Other) const;
		void			operator*=(const Quat& Other);

		static bool AnyHasNegativeScale(const Vector& InScale3D, const Vector& InOtherScale3D);
		void ScaleTranslation(const Vector& InScale3D);
		void ScaleTranslation(const float& Scale);
		void RemoveScaling(float Tolerance = SMALL_NUMBER);
		float GetMaximumAxisScale() const;
		float GetMinimumAxisScale() const;
		// Inverse does not work well with VQS format(in particular non-uniform), so removing it, but made two below functions to be used instead.

		/*******************************************************************************************
		 * The below 2 functions are the ones to get delta transform and return Transform format that can be concatenated
		 * Inverse itself can't concatenate with VQS format(since VQS always transform from S->Q->T, where inverse happens from T(-1)->Q(-1)->S(-1))
		 * So these 2 provides ways to fix this
		 * GetRelativeTransform returns this*Other(-1) and parameter is Other(not Other(-1))
		 * GetRelativeTransformReverse returns this(-1)*Other, and parameter is Other.
		 *******************************************************************************************/
		Transform GetRelativeTransform(const Transform& Other) const;
		Transform GetRelativeTransformReverse(const Transform& Other) const;
		/**
		 * Set current transform and the relative to ParentTransform.
		 * Equates to This = This->GetRelativeTransform(Parent), but saves the intermediate Transform storage and copy.
		 */
		void		SetToRelativeTransform(const Transform& ParentTransform);

		Vector4	TransformVector4(const Vector4& V) const;
		Vector4	TransformVector4NoScale(const Vector4& V) const;
		Vector		TransformPosition(const Vector& V) const;
		Vector		TransformPositionNoScale(const Vector& V) const;

		/** Inverts the transform and then transforms V - correctly handles scaling in this transform. */
		Vector		InverseTransformPosition(const Vector &V) const;
		Vector		InverseTransformPositionNoScale(const Vector &V) const;
		Vector		TransformVector(const Vector& V) const;
		Vector		TransformVectorNoScale(const Vector& V) const;

		/**
		 *	Transform a direction vector by the inverse of this matrix - will not take into account translation part.
		 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT with adjoint of matrix inverse.
		 */
		Vector InverseTransformVector(const Vector &V) const;
		Vector InverseTransformVectorNoScale(const Vector &V) const;

		/**
		* Transform a rotation.
		* For example if this is a LocalToWorld transform, TransformRotation(Q) would transform Q from local to world space.
		*/
		Quat TransformRotation(const Quat& Q) const;

		/**
		* Inverse transform a rotation.
		* For example if this is a LocalToWorld transform, InverseTransformRotation(Q) would transform Q from world to local space.
		*/
		Quat InverseTransformRotation(const Quat& Q) const;

		Transform	GetScaled(float Scale) const;
		Transform	GetScaled(Vector Scale) const;
		Vector		GetScaledAxis(Axis InAxis) const;
		Vector		GetUnitAxis(Axis InAxis) const;
		void		Mirror(Axis MirrorAxis, Axis FlipAxis);
		static Vector	GetSafeScaleReciprocal(const Vector& InScale, float Tolerance = SMALL_NUMBER);

		Vector GetLocation() const;

		oocd::Rotator Rotator() const;

		/** Calculate the  */
		float GetDeterminant() const;

		/** Set the translation of this transformation */
		void SetLocation(const Vector& Origin);

		/**
		 * Checks the components for NaN's
		 * @return Returns true if any component (rotation, translation, or scale) is a NAN
		 */
		bool ContainsNaN() const;

		inline bool IsValid() const
		{
			if (ContainsNaN())
			{
				return false;
			}

			if (!IsRotationNormalized())
			{
				return false;
			}

			return true;
		}

		// Binary comparison operators.
		/*
		bool operator==( const Transform& Other ) const
		{
			return Rotation==Other.Rotation && Translation==Other.Translation && Scale3D==Other.Scale3D;
		}
		bool operator!=( const Transform& Other ) const
		{
			return Rotation!=Other.Rotation || Translation!=Other.Translation || Scale3D!=Other.Scale3D;
		}
		*/

	private:

		static bool Private_AnyHasNegativeScale(const VectorRegister& InScale3D, const  VectorRegister& InOtherScale3D);

		bool Private_RotationEquals(const VectorRegister& InRotation, const ScalarRegister& Tolerance = ScalarRegister(GlobalVectorConstants::KindaSmallNumber)) const;

		bool Private_TranslationEquals(const VectorRegister& InTranslation, const ScalarRegister& Tolerance = ScalarRegister(GlobalVectorConstants::KindaSmallNumber)) const;

		bool Private_Scale3DEquals(const VectorRegister& InScale3D, const ScalarRegister& Tolerance = ScalarRegister(GlobalVectorConstants::KindaSmallNumber)) const;

	public:

		// Test if A's rotation equals B's rotation, within a tolerance. Preferred over "A.GetRotation().Equals(B.GetRotation())" because it is faster on some platforms.
		static bool AreRotationsEqual(const Transform& A, const Transform& B, float Tolerance = KINDA_SMALL_NUMBER);

		// Test if A's translation equals B's translation, within a tolerance. Preferred over "A.GetTranslation().Equals(B.GetTranslation())" because it avoids VectorRegister->Vector conversion.
		static bool AreTranslationsEqual(const Transform& A, const Transform& B, float Tolerance = KINDA_SMALL_NUMBER);

		// Test if A's scale equals B's scale, within a tolerance. Preferred over "A.GetScale3D().Equals(B.GetScale3D())" because it avoids VectorRegister->Vector conversion.
		static bool AreScale3DsEqual(const Transform& A, const Transform& B, float Tolerance = KINDA_SMALL_NUMBER);

		// Test if this Transform's rotation equals another's rotation, within a tolerance. Preferred over "GetRotation().Equals(Other.GetRotation())" because it is faster on some platforms.
		bool RotationEquals(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const;

		// Test if this Transform's translation equals another's translation, within a tolerance. Preferred over "GetTranslation().Equals(Other.GetTranslation())" because it avoids VectorRegister->Vector conversion.
		bool TranslationEquals(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const;

		// Test if this Transform's scale equals another's scale, within a tolerance. Preferred over "GetScale3D().Equals(Other.GetScale3D())" because it avoids VectorRegister->Vector conversion.
		bool Scale3DEquals(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const;

		// Test if all components of the transforms are equal, within a tolerance.
		bool Equals(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const;

		// Test if rotation and translation components of the transforms are equal, within a tolerance.
		bool EqualsNoScale(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const;

		static void Multiply(Transform* OutTransform, const Transform* A, const Transform* B);
		/**
		 * Sets the components
		 * @param InRotation The new value for the Rotation component
		 * @param InTranslation The new value for the Translation component
		 * @param InScale3D The new value for the Scale3D component
		 */
		void SetComponents(const Quat& InRotation, const Vector& InTranslation, const Vector& InScale3D);

		/**
		 * Sets the components to the identity transform:
		 *   Rotation = (0,0,0,1)
		 *   Translation = (0,0,0)
		 *   Scale3D = (1,1,1)
		 */
		void SetIdentity();

		/**
		 * Scales the Scale3D component by a new factor
		 * @param Scale3DMultiplier The value to multiply Scale3D with
		 */
		void MultiplyScale3D(const Vector& Scale3DMultiplier);

		/**
		 * Sets the translation component
		 * @param NewTranslation The new value for the translation component
		 */
		void SetTranslation(const Vector& NewTranslation);

		/** Copy translation from another Transform. */
		void CopyTranslation(const Transform& Other);

		/**
		 * Concatenates another rotation to this transformation
		 * @param DeltaRotation The rotation to concatenate in the following fashion: Rotation = Rotation * DeltaRotation
		 */
		void ConcatenateRotation(const Quat& DeltaRotation);

		/**
		 * Adjusts the translation component of this transformation
		 * @param DeltaTranslation The translation to add in the following fashion: Translation += DeltaTranslation
		 */
		void AddToTranslation(const Vector& DeltaTranslation);

		/**
		 * Add the translations from two Transforms and return the result.
		 * @return A.Translation + B.Translation
		 */
		static Vector AddTranslations(const Transform& A, const Transform& B);

		/**
		 * Subtract translations from two Transforms and return the difference.
		 * @return A.Translation - B.Translation.
		 */
		static Vector SubtractTranslations(const Transform& A, const Transform& B);

		/**
		 * Sets the rotation component
		 * @param NewRotation The new value for the rotation component
		 */
		void SetRotation(const Quat& NewRotation);

		/** Copy rotation from another Transform. */
		void CopyRotation(const Transform& Other);

		/**
		 * Sets the Scale3D component
		 * @param NewScale3D The new value for the Scale3D component
		 */
		void SetScale3D(const Vector& NewScale3D);

		/** Copy scale from another Transform. */
		void CopyScale3D(const Transform& Other);

		/**
		 * Sets both the translation and Scale3D components at the same time
		 * @param NewTranslation The new value for the translation component
		 * @param NewScale3D The new value for the Scale3D component
		 */
		void SetTranslationAndScale3D(const Vector& NewTranslation, const Vector& NewScale3D);

		/** @note : Added template type function for Accumulate
		  * The template type isn't much useful yet, but it is with the plan to move forward
		  * to unify blending features with just type of additive or full pose
		  * Eventually it would be nice to just call blend and it all works depending on full pose
		  * or additive, but right now that is a lot more refactoring
		  * For now this types only defines the different functionality of accumulate
		  */

		  /**
		  * Accumulates another transform with this one
		  *
		  * Rotation is accumulated multiplicatively (Rotation = SourceAtom.Rotation * Rotation)
		  * Translation is accumulated additively (Translation += SourceAtom.Translation)
		  * Scale3D is accumulated multiplicatively (Scale3D *= SourceAtom.Scale3D)
		  *
		  * @param SourceAtom The other transform to accumulate into this one
		  */
		void Accumulate(const Transform& SourceAtom);

		/**
		* Accumulates another transform with this one, with a blending weight
		*
		* Let SourceAtom = Atom * BlendWeight
		* Rotation is accumulated multiplicatively (Rotation = SourceAtom.Rotation * Rotation).
		* Translation is accumulated additively (Translation += SourceAtom.Translation)
		* Scale3D is accumulated multiplicatively (Scale3D *= SourceAtom.Scale3D)
		*
		* Note: Rotation will not be normalized! Will have to be done manually.
		*
		* @param Atom The other transform to accumulate into this one
		* @param BlendWeight The weight to multiply Atom by before it is accumulated.
		*/
		void Accumulate(const Transform& Atom, const ScalarRegister& BlendWeight);
		/**
		 * Accumulates another transform with this one, with an optional blending weight
		 *
		 * Rotation is accumulated additively, in the shortest direction (Rotation = Rotation +/- DeltaAtom.Rotation * Weight)
		 * Translation is accumulated additively (Translation += DeltaAtom.Translation * Weight)
		 * Scale3D is accumulated additively (Scale3D += DeltaAtom.Scale * Weight)
		 *
		 * @param DeltaAtom The other transform to accumulate into this one
		 * @param Weight The weight to multiply DeltaAtom by before it is accumulated.
		 */
		void AccumulateWithShortestRotation(const Transform& DeltaAtom, const ScalarRegister& BlendWeight);

		/** Accumulates another transform with this one, with a blending weight
		*
		* Let SourceAtom = Atom * BlendWeight
		* Rotation is accumulated multiplicatively (Rotation = SourceAtom.Rotation * Rotation).
		* Translation is accumulated additively (Translation += SourceAtom.Translation)
		* Scale3D is accumulated assuming incoming scale is additive scale (Scale3D *= (1 + SourceAtom.Scale3D))
		*
		* When we create additive, we create additive scale based on [TargetScale/SourceScale -1]
		* because that way when you apply weight of 0.3, you don't shrink. We only saves the % of grow/shrink
		* when we apply that back to it, we add back the 1, so that it goes back to it.
		* This solves issue where you blend two additives with 0.3, you don't come back to 0.6 scale, but 1 scale at the end
		* because [1 + [1-1]*0.3 + [1-1]*0.3] becomes 1, so you don't shrink by applying additive scale
		*
		* Note: Rotation will not be normalized! Will have to be done manually.
		*
		* @param Atom The other transform to accumulate into this one
		* @param BlendWeight The weight to multiply Atom by before it is accumulated.
		*/
		void AccumulateWithAdditiveScale(const Transform& Atom, const ScalarRegister& BlendWeight);

		/**
		 * Set the translation and Scale3D components of this transform to a linearly interpolated combination of two other transforms
		 *
		 * Translation = FMath::Lerp(SourceAtom1.Translation, SourceAtom2.Translation, Alpha)
		 * Scale3D = FMath::Lerp(SourceAtom1.Scale3D, SourceAtom2.Scale3D, Alpha)
		 *
		 * @param SourceAtom1 The starting point source atom (used 100% if Alpha is 0)
		 * @param SourceAtom2 The ending point source atom (used 100% if Alpha is 1)
		 * @param Alpha The blending weight between SourceAtom1 and SourceAtom2
		 */
		void LerpTranslationScale3D(const Transform& SourceAtom1, const Transform& SourceAtom2, const ScalarRegister& Alpha);

		/**
		 * Normalize the rotation component of this transformation
		 */
		void NormalizeRotation()
		{
			MRotation = VectorNormalizeQuaternion(MRotation);
		}

		/**
		 * Checks whether the rotation component is normalized or not
		 *
		 * @return true if the rotation component is normalized, and false otherwise.
		 */
		bool IsRotationNormalized() const;

		/**
		 * Blends the Identity transform with a weighted source transform and accumulates that into a destination transform
		 *
		 * SourceAtom = Blend(Identity, SourceAtom, BlendWeight)
		 * FinalAtom.Rotation = SourceAtom.Rotation * FinalAtom.Rotation
		 * FinalAtom.Translation += SourceAtom.Translation
		 * FinalAtom.Scale3D *= SourceAtom.Scale3D
		 *
		 * @param FinalAtom [in/out] The atom to accumulate the blended source atom into
		 * @param SourceAtom The target transformation (used when BlendWeight = 1)
		 * @param Alpha The blend weight between Identity and SourceAtom
		 */
		static void BlendFromIdentityAndAccumulate(Transform& FinalAtom, Transform& SourceAtom, const ScalarRegister& BlendWeight);

		/**
		 * Returns the rotation component
		 *
		 * @return The rotation component
		 */
		Quat GetRotation() const;

		/**
		 * Returns the translation component
		 *
		 * @return The translation component
		 */
		Vector GetTranslation() const;

		/**
		 * Returns the Scale3D component
		 *
		 * @return The Scale3D component
		 */
		Vector GetScale3D() const;

		/**
		 * Sets the Rotation and Scale3D of this transformation from another transform
		 *
		 * @param SrcBA The transform to copy rotation and Scale3D from
		 */
		void CopyRotationPart(const Transform& SrcBA);

		/**
		 * Sets the Translation and Scale3D of this transformation from another transform
		 *
		 * @param SrcBA The transform to copy translation and Scale3D from
		 */
		void CopyTranslationAndScale3D(const Transform& SrcBA);

		void SetFromMatrix(const Matrix& InMatrix);

	private:
		void ToMatrixInternal(VectorRegister& OutDiagonals, VectorRegister& OutAdds, VectorRegister& OutSubtracts) const;

		void ToMatrixInternalNoScale(VectorRegister& OutDiagonals, VectorRegister& OutAdds, VectorRegister& OutSubtracts) const;

		/**
		 * mathematically if you have 0 scale, it should be infinite,
		 * however, in practice if you have 0 scale, and relative transform doesn't make much sense
		 * anymore because you should be instead of showing gigantic infinite mesh
		 * also returning BIG_NUMBER causes sequential NaN issues by multiplying
		 * so we hardcode as 0
		 */
		static   VectorRegister		GetSafeScaleReciprocal(const VectorRegister& InScale, const ScalarRegister& Tolerance = ScalarRegister(GlobalVectorConstants::SmallNumber));

		//逆变换
		Transform InverseFast() const;

		//使用矩阵创建一个新变换：OutTransform = A * B，同时保持A和B给出的比例
		static void MultiplyUsingMatrixWithScale(Transform* OutTransform, const Transform* A, const Transform* B);
		//使用所需比例从给定矩阵（AMatrix * BMatrix）的乘法创建新变换
		static void ConstructTransformFromMatrixWithDesiredScale(const Matrix& AMatrix, const Matrix& BMatrix, const VectorRegister& DesiredScale, Transform& OutTransform);
		//使得创造一个新的变换，scale相对不变
		static void GetRelativeTransformUsingMatrixWithScale(Transform* OutTransform, const Transform* Base, const Transform* Relative);
	};
};
