// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Type.h"
#include "Vector.h"
#include "AssertionMacros.h"
#include "VectorRegister.h"
#include "ScalarRegister.h"
#include "Matrix.h"
namespace oocd
{

	struct Transform
	{
		friend struct Z_Construct_UScriptStruct_Transform_Statics;

	protected:
		//旋转
		Quat	MRotation;
		//位置
		Vector	Translation;
		//大小
		Vector	Scale3D;
	public:
		//默认
		static  const Transform Identity;
		void DiagnosticCheckNaN_Translate() const {}
		void DiagnosticCheckNaN_Rotate() const {}
		void DiagnosticCheckNaN_Scale3D() const {}
		void DiagnosticCheckNaN_All() const {}
		void DiagnosticCheck_IsValid() const {}


		Transform();

		
		explicit Transform(const Vector& InTranslation);

		explicit Transform(const Quat& InRotation);

		explicit Transform(const oocd::Rotator& InRotation);

	
		Transform(const Quat& InRotation, const Vector& InTranslation, const Vector& InScale3D = Vector::OneVector);

	
		Transform(const Rotator& InRotation, const Vector& InTranslation, const Vector& InScale3D = Vector::OneVector);

	
		Transform(const Transform& InTransform);

		//转换构造
		explicit Transform(const Matrix& InMatrix)
		{
			SetFromMatrix(InMatrix);
			DiagnosticCheckNaN_All();
		}
		Transform(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InTranslation)
		{
			SetFromMatrix(Matrix(InX, InY, InZ, InTranslation));
			DiagnosticCheckNaN_All();
		}
		//得到大小matrix
		Matrix ToMatrixWithScale() const;

		Matrix ToInverseMatrixWithScale() const
		{
			// todo: optimize
			return ToMatrixWithScale().Inverse();
		}

		//得到逆变换
		Transform Inverse() const;

		//得到忽略scale的矩阵
		Matrix ToMatrixNoScale() const;

		//结合两个transform
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
			return Transform(MRotation + Atom.MRotation, Translation + Atom.Translation, Scale3D + Atom.Scale3D);
		}

		Transform& operator+=(const Transform& Atom)
		{
			Translation += Atom.Translation;

			MRotation.X += Atom.MRotation.X;
			MRotation.Y += Atom.MRotation.Y;
			MRotation.Z += Atom.MRotation.Z;
			MRotation.W += Atom.MRotation.W;

			Scale3D += Atom.Scale3D;

			return *this;
		}

		Transform operator*(ScalarRegister Mult) const
		{
			return Transform(MRotation * Mult, Translation * Mult, Scale3D * Mult);
		}

		Transform& operator*=(ScalarRegister Mult)
		{
			Translation *= Mult;
			MRotation.X *= Mult;
			MRotation.Y *= Mult;
			MRotation.Z *= Mult;
			MRotation.W *= Mult;
			Scale3D *= Mult;

			return *this;
		}

		/**
		* Return a transform that is the result of this multiplied by another transform.
		* Order matters when composing transforms : C = A * B will yield a transform C that logically first applies A then B to any subsequent transformation.
		*
		* @param  Other other transform by which to multiply.
		* @return new transform: this * Other
		*/
		Transform operator*(const Transform& Other) const;


		/**
		* Sets this transform to the result of this multiplied by another transform.
		* Order matters when composing transforms : C = A * B will yield a transform C that logically first applies A then B to any subsequent transformation.
		*
		* @param  Other other transform by which to multiply.
		*/
		void operator*=(const Transform& Other);

		/**
		* Return a transform that is the result of this multiplied by another transform (made only from a rotation).
		* Order matters when composing transforms : C = A * B will yield a transform C that logically first applies A then B to any subsequent transformation.
		*
		* @param  Other other quaternion rotation by which to multiply.
		* @return new transform: this * Transform(Other)
		*/
		Transform operator*(const Quat& Other) const;

		/**
		* Sets this transform to the result of this multiplied by another transform (made only from a rotation).
		* Order matters when composing transforms : C = A * B will yield a transform C that logically first applies A then B to any subsequent transformation.
		*
		* @param  Other other quaternion rotation by which to multiply.
		*/
		void operator*=(const Quat& Other);

		static bool AnyHasNegativeScale(const Vector& InScale3D, const  Vector& InOtherScale3D);
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
		void SetToRelativeTransform(const Transform& ParentTransform);

		Vector4 TransformVector4(const Vector4& V) const;
		Vector4 TransformVector4NoScale(const Vector4& V) const;
		Vector TransformPosition(const Vector& V) const;
		Vector TransformPositionNoScale(const Vector& V) const;

		/** Inverts the transform and then transforms V - correctly handles scaling in this transform. */
		Vector InverseTransformPosition(const Vector &V) const;
		Vector InverseTransformPositionNoScale(const Vector &V) const;
		Vector TransformVector(const Vector& V) const;
		Vector TransformVectorNoScale(const Vector& V) const;

		/**
		*	Transform a direction vector by the inverse of this transform - will not take into account translation part.
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

		Transform GetScaled(float Scale) const;
		Transform GetScaled(Vector Scale) const;
		Vector GetScaledAxis(Axis InAxis) const;
		Vector GetUnitAxis(Axis InAxis) const;
		void Mirror(Axis MirrorAxis, Axis FlipAxis);
		//得到倒数
		static Vector GetSafeScaleReciprocal(const Vector& InScale, float Tolerance = SMALL_NUMBER);

		// temp function for easy conversion
		Vector GetLocation() const
		{
			return GetTranslation();
		}

		Rotator Rotator() const
		{
			return MRotation.Rotator();
		}

		/** Calculate the determinant of this transformation */
		float GetDeterminant() const
		{
			return Scale3D.X * Scale3D.Y * Scale3D.Z;
		}

		/** Set the translation of this transformation */
		void SetLocation(const Vector& Origin)
		{
			Translation = Origin;
			DiagnosticCheckNaN_Translate();
		}

	

	// Binary comparison operators.
		/*
		bool operator==(const Transform& Other) const
		{
		return Rotation==Other.Rotation && Translation==Other.Translation && Scale3D==Other.Scale3D;
		}
		bool operator!=(const Transform& Other) const
		{
		return Rotation!=Other.Rotation || Translation!=Other.Translation || Scale3D!=Other.Scale3D;
		}
		*/

	private:

		bool Private_RotationEquals(const Quat& InRotation, const float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return MRotation.Equals(InRotation, Tolerance);
		}

		bool Private_TranslationEquals(const Vector& InTranslation, const float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return Translation.Equals(InTranslation, Tolerance);
		}

		bool Private_Scale3DEquals(const Vector& InScale3D, const float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return Scale3D.Equals(InScale3D, Tolerance);
		}

	public:

		// Test if A's rotation equals B's rotation, within a tolerance. Preferred over "A.GetRotation().Equals(B.GetRotation())" because it is faster on some platforms.
		static bool AreRotationsEqual(const Transform& A, const Transform& B, float Tolerance = KINDA_SMALL_NUMBER)
		{
			return A.Private_RotationEquals(B.MRotation, Tolerance);
		}

		// Test if A's translation equals B's translation, within a tolerance. Preferred over "A.GetTranslation().Equals(B.GetTranslation())" because it is faster on some platforms.
		static bool AreTranslationsEqual(const Transform& A, const Transform& B, float Tolerance = KINDA_SMALL_NUMBER)
		{
			return A.Private_TranslationEquals(B.Translation, Tolerance);
		}

		// Test if A's scale equals B's scale, within a tolerance. Preferred over "A.GetScale3D().Equals(B.GetScale3D())" because it is faster on some platforms.
		static bool AreScale3DsEqual(const Transform& A, const Transform& B, float Tolerance = KINDA_SMALL_NUMBER)
		{
			return A.Private_Scale3DEquals(B.Scale3D, Tolerance);
		}



		// Test if this Transform's rotation equals another's rotation, within a tolerance. Preferred over "GetRotation().Equals(Other.GetRotation())" because it is faster on some platforms.
		bool RotationEquals(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return AreRotationsEqual(*this, Other, Tolerance);
		}

		// Test if this Transform's translation equals another's translation, within a tolerance. Preferred over "GetTranslation().Equals(Other.GetTranslation())" because it is faster on some platforms.
		bool TranslationEquals(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return AreTranslationsEqual(*this, Other, Tolerance);
		}

		// Test if this Transform's scale equals another's scale, within a tolerance. Preferred over "GetScale3D().Equals(Other.GetScale3D())" because it is faster on some platforms.
		bool Scale3DEquals(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return AreScale3DsEqual(*this, Other, Tolerance);
		}


		// Test if all components of the transforms are equal, within a tolerance.
		bool Equals(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return Private_TranslationEquals(Other.Translation, Tolerance) && Private_RotationEquals(Other.MRotation, Tolerance) && Private_Scale3DEquals(Other.Scale3D, Tolerance);
		}

		// Test if rotation and translation components of the transforms are equal, within a tolerance.
		bool EqualsNoScale(const Transform& Other, float Tolerance = KINDA_SMALL_NUMBER) const
		{
			return Private_TranslationEquals(Other.Translation, Tolerance) && Private_RotationEquals(Other.MRotation, Tolerance);
		}

		/**
		* Create a new transform: OutTransform = A * B.
		*
		* Order matters when composing transforms : A * B will yield a transform that logically first applies A then B to any subsequent transformation.
		*
		* @param  OutTransform pointer to transform that will store the result of A * B.
		* @param  A Transform A.
		* @param  B Transform B.
		*/
		static void Multiply(Transform* OutTransform, const Transform* A, const Transform* B);

		/**
		* Sets the components
		* @param InRotation The new value for the Rotation component
		* @param InTranslation The new value for the Translation component
		* @param InScale3D The new value for the Scale3D component
		*/
		void SetComponents(const Quat& InRotation, const Vector& InTranslation, const Vector& InScale3D)
		{
			MRotation = InRotation;
			Translation = InTranslation;
			Scale3D = InScale3D;

			DiagnosticCheckNaN_All();
		}

		/**
		* Sets the components to the identity transform:
		*   Rotation = (0,0,0,1)
		*   Translation = (0,0,0)
		*   Scale3D = (1,1,1)
		*/
		void SetIdentity()
		{
			MRotation = Quat::Identity;
			Translation = Vector::ZeroVector;
			Scale3D = Vector(1, 1, 1);
		}

		/**
		* Scales the Scale3D component by a new factor
		* @param Scale3DMultiplier The value to multiply Scale3D with
		*/
		void MultiplyScale3D(const Vector& Scale3DMultiplier)
		{
			Scale3D *= Scale3DMultiplier;
			DiagnosticCheckNaN_Scale3D();
		}

		/**
		* Sets the translation component
		* @param NewTranslation The new value for the translation component
		*/
		void SetTranslation(const Vector& NewTranslation)
		{
			Translation = NewTranslation;
			DiagnosticCheckNaN_Translate();
		}

		/** Copy translation from another Transform. */
		void CopyTranslation(const Transform& Other)
		{
			Translation = Other.Translation;
		}

		/**
		* Concatenates another rotation to this transformation
		* @param DeltaRotation The rotation to concatenate in the following fashion: Rotation = Rotation * DeltaRotation
		*/
		void ConcatenateRotation(const Quat& DeltaRotation)
		{
			MRotation = MRotation * DeltaRotation;
			DiagnosticCheckNaN_Rotate();
		}

		/**
		* Adjusts the translation component of this transformation
		* @param DeltaTranslation The translation to add in the following fashion: Translation += DeltaTranslation
		*/
		void AddToTranslation(const Vector& DeltaTranslation)
		{
			Translation += DeltaTranslation;
			DiagnosticCheckNaN_Translate();
		}

		/**
		* Add the translations from two Transforms and return the result.
		* @return A.Translation + B.Translation
		*/
		static Vector AddTranslations(const Transform& A, const Transform& B)
		{
			return A.Translation + B.Translation;
		}

		/**
		* Subtract translations from two Transforms and return the difference.
		* @return A.Translation - B.Translation.
		*/
		static Vector SubtractTranslations(const Transform& A, const Transform& B)
		{
			return A.Translation - B.Translation;
		}

		/**
		* Sets the rotation component
		* @param NewRotation The new value for the rotation component
		*/
		void SetRotation(const Quat& NewRotation)
		{
			MRotation = NewRotation;
			DiagnosticCheckNaN_Rotate();
		}

		/** Copy rotation from another Transform. */
		void CopyRotation(const Transform& Other)
		{
			MRotation = Other.MRotation;
		}

		/**
		* Sets the Scale3D component
		* @param NewScale3D The new value for the Scale3D component
		*/
		void SetScale3D(const Vector& NewScale3D)
		{
			Scale3D = NewScale3D;
			DiagnosticCheckNaN_Scale3D();
		}

		/** Copy scale from another Transform. */
		void CopyScale3D(const Transform& Other)
		{
			Scale3D = Other.Scale3D;
		}

		/**
		* Sets both the translation and Scale3D components at the same time
		* @param NewTranslation The new value for the translation component
		* @param NewScale3D The new value for the Scale3D component
		*/
		void SetTranslationAndScale3D(const Vector& NewTranslation, const Vector& NewScale3D)
		{
			Translation = NewTranslation;
			Scale3D = NewScale3D;

			DiagnosticCheckNaN_Translate();
			DiagnosticCheckNaN_Scale3D();
		}

		/** @note: Added template type function for Accumulate
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
		void Accumulate(const Transform& SourceAtom)
		{
			// Add ref pose relative animation to base animation, only if rotation is significant.
			if (Math::Square(SourceAtom.MRotation.W) < 1.f - DELTA * DELTA)
			{
				MRotation = SourceAtom.MRotation * MRotation;
			}

			Translation += SourceAtom.Translation;
			Scale3D *= SourceAtom.Scale3D;

			DiagnosticCheckNaN_All();

			checkSlow(IsRotationNormalized());
		}

		/** Accumulates another transform with this one, with a blending weight
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
		void Accumulate(const Transform& Atom, float BlendWeight/* default param doesn't work since vectorized version takes ref param */)
		{
			Transform SourceAtom(Atom * BlendWeight);

			// Add ref pose relative animation to base animation, only if rotation is significant.
			if (Math::Square(SourceAtom.MRotation.W) < 1.f - DELTA * DELTA)
			{
				MRotation = SourceAtom.MRotation * MRotation;
			}

			Translation += SourceAtom.Translation;
			Scale3D *= SourceAtom.Scale3D;

			DiagnosticCheckNaN_All();
		}

		/**
		* Accumulates another transform with this one, with an optional blending weight
		*
		* Rotation is accumulated additively, in the shortest direction (Rotation = Rotation +/- DeltaAtom.Rotation * Weight)
		* Translation is accumulated additively (Translation += DeltaAtom.Translation * Weight)
		* Scale3D is accumulated additively (Scale3D += DeltaAtom.Scale3D * Weight)
		*
		* @param DeltaAtom The other transform to accumulate into this one
		* @param Weight The weight to multiply DeltaAtom by before it is accumulated.
		*/
		void AccumulateWithShortestRotation(const Transform& DeltaAtom, float BlendWeight/* default param doesn't work since vectorized version takes ref param */)
		{
			Transform Atom(DeltaAtom * BlendWeight);

			// To ensure the 'shortest route', we make sure the dot product between the accumulator and the incoming child atom is positive.
			if ((Atom.MRotation | MRotation) < 0.f)
			{
				MRotation.X -= Atom.MRotation.X;
				MRotation.Y -= Atom.MRotation.Y;
				MRotation.Z -= Atom.MRotation.Z;
				MRotation.W -= Atom.MRotation.W;
			}
			else
			{
				MRotation.X += Atom.MRotation.X;
				MRotation.Y += Atom.MRotation.Y;
				MRotation.Z += Atom.MRotation.Z;
				MRotation.W += Atom.MRotation.W;
			}

			Translation += Atom.Translation;
			Scale3D += Atom.Scale3D;

			DiagnosticCheckNaN_All();
		}

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
		void AccumulateWithAdditiveScale(const Transform& Atom, float BlendWeight/* default param doesn't work since vectorized version takes ref param */)
		{
			const Vector DefaultScale(Vector::OneVector);

			Transform SourceAtom(Atom * BlendWeight);

			// Add ref pose relative animation to base animation, only if rotation is significant.
			if (Math::Square(SourceAtom.MRotation.W) < 1.f - DELTA * DELTA)
			{
				MRotation = SourceAtom.MRotation * MRotation;
			}

			Translation += SourceAtom.Translation;
			Scale3D *= (DefaultScale + SourceAtom.Scale3D);

			DiagnosticCheckNaN_All();
		}
		/**
		* Set the translation and Scale3D components of this transform to a linearly interpolated combination of two other transforms
		*
		* Translation = Math::Lerp(SourceAtom1.Translation, SourceAtom2.Translation, Alpha)
		* Scale3D = Math::Lerp(SourceAtom1.Scale3D, SourceAtom2.Scale3D, Alpha)
		*
		* @param SourceAtom1 The starting point source atom (used 100% if Alpha is 0)
		* @param SourceAtom2 The ending point source atom (used 100% if Alpha is 1)
		* @param Alpha The blending weight between SourceAtom1 and SourceAtom2
		*/
		void LerpTranslationScale3D(const Transform& SourceAtom1, const Transform& SourceAtom2, ScalarRegister Alpha)
		{
			Translation = Math::Lerp(SourceAtom1.Translation, SourceAtom2.Translation, Alpha);
			Scale3D = Math::Lerp(SourceAtom1.Scale3D, SourceAtom2.Scale3D, Alpha);

			DiagnosticCheckNaN_Translate();
			DiagnosticCheckNaN_Scale3D();
		}

		/**
		* Normalize the rotation component of this transformation
		*/
		void NormalizeRotation()
		{
			MRotation.Normalize();
			DiagnosticCheckNaN_Rotate();
		}

		/**
		* Checks whether the rotation component is normalized or not
		*
		* @return true if the rotation component is normalized, and false otherwise.
		*/
		bool IsRotationNormalized() const
		{
			return MRotation.IsNormalized();
		}

		/**
		* Blends the Identity transform with a weighted source transform and accumulates that into a destination transform
		*
		* SourceAtom = Blend(Identity, SourceAtom, BlendWeight)
		* FinalAtom.Rotation = SourceAtom.Rotation * FinalAtom.Rotation
		* FinalAtom.Translation += SourceAtom.Translation
		* FinalAtom.Scale3D *= SourceAtom.Scale3D
		*
		* @param FinalAtom [in/out] The atom to accumulate the blended source atom into
		* @param SourceAtom The target transformation (used when BlendWeight = 1); this is modified during the process
		* @param BlendWeight The blend weight between Identity and SourceAtom
		*/
		static void BlendFromIdentityAndAccumulate(Transform& FinalAtom, Transform& SourceAtom, float BlendWeight)
		{
			const  Transform AdditiveIdentity(Quat::Identity, Vector::ZeroVector, Vector::ZeroVector);
			const Vector DefaultScale(Vector::OneVector);

			// Scale delta by weight
			if (BlendWeight < (1.f - ZERO_ANIMWEIGHT_THRESH))
			{
				SourceAtom.Blend(AdditiveIdentity, SourceAtom, BlendWeight);
			}

			// Add ref pose relative animation to base animation, only if rotation is significant.
			if (Math::Square(SourceAtom.MRotation.W) < 1.f - DELTA * DELTA)
			{
				FinalAtom.MRotation = SourceAtom.MRotation * FinalAtom.MRotation;
			}

			FinalAtom.Translation += SourceAtom.Translation;
			FinalAtom.Scale3D *= (DefaultScale + SourceAtom.Scale3D);

			checkSlow(FinalAtom.IsRotationNormalized());
		}

		/**
		* Returns the rotation component
		*
		* @return The rotation component
		*/
		Quat GetRotation() const
		{
			DiagnosticCheckNaN_Rotate();
			return MRotation;
		}

		/**
		* Returns the translation component
		*
		* @return The translation component
		*/
		Vector GetTranslation() const
		{
			DiagnosticCheckNaN_Translate();
			return Translation;
		}

		/**
		* Returns the Scale3D component
		*
		* @return The Scale3D component
		*/
		Vector GetScale3D() const
		{
			DiagnosticCheckNaN_Scale3D();
			return Scale3D;
		}

		/**
		* Sets the Rotation and Scale3D of this transformation from another transform
		*
		* @param SrcBA The transform to copy rotation and Scale3D from
		*/
		void CopyRotationPart(const Transform& SrcBA)
		{
			MRotation = SrcBA.MRotation;
			Scale3D = SrcBA.Scale3D;

			DiagnosticCheckNaN_Rotate();
			DiagnosticCheckNaN_Scale3D();
		}

		/**
		* Sets the Translation and Scale3D of this transformation from another transform
		*
		* @param SrcBA The transform to copy translation and Scale3D from
		*/
		void CopyTranslationAndScale3D(const Transform& SrcBA)
		{
			Translation = SrcBA.Translation;
			Scale3D = SrcBA.Scale3D;

			DiagnosticCheckNaN_Translate();
			DiagnosticCheckNaN_Scale3D();
		}

		void SetFromMatrix(const Matrix& InMatrix)
		{
			Matrix M = InMatrix;

			// Get the 3D scale from the matrix
			Scale3D = M.ExtractScaling();

			// If there is negative scaling going on, we handle that here
			if (InMatrix.Determinant() < 0.f)
			{
				// Assume it is along X and modify transform accordingly. 
				// It doesn't actually matter which axis we choose, the 'appearance' will be the same
				Scale3D.X *= -1.f;
				M.SetAxis(0, -M.GetScaledAxis(Axis::X));
			}

			MRotation = Quat(M);
			Translation = InMatrix.GetOrigin();

			// Normalize rotation
			MRotation.Normalize();
		}

	private:
		/**
		* Create a new transform: OutTransform = A * B using the matrix while keeping the scale that's given by A and B
		* Please note that this operation is a lot more expensive than normal Multiply
		*
		* Order matters when composing transforms : A * B will yield a transform that logically first applies A then B to any subsequent transformation.
		*
		* @param  OutTransform pointer to transform that will store the result of A * B.
		* @param  A Transform A.
		* @param  B Transform B.
		*/
		static void MultiplyUsingMatrixWithScale(Transform* OutTransform, const Transform* A, const Transform* B);
		/**
		* Create a new transform from multiplications of given to matrices (AMatrix*BMatrix) using desired scale
		* This is used by MultiplyUsingMatrixWithScale and GetRelativeTransformUsingMatrixWithScale
		* This is only used to handle negative scale
		*
		* @param	AMatrix first Matrix of operation
		* @param	BMatrix second Matrix of operation
		* @param	DesiredScale - there is no check on if the magnitude is correct here. It assumes that is correct.
		* @param	OutTransform the constructed transform
		*/
		static void ConstructTransformFromMatrixWithDesiredScale(const Matrix& AMatrix, const Matrix& BMatrix, const Vector& DesiredScale, Transform& OutTransform);
	
		static void GetRelativeTransformUsingMatrixWithScale(Transform* OutTransform, const Transform* Base, const Transform* Relative);
	};

}








