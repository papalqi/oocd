#pragma once
#include "Rotator.h"
#include "Vector3.h"
#include "Matrix.h"
namespace oocd
{
	class Quat
	{
	public:

		float X;

		float Y;

		float Z;

		float W;

	public:

		static  const Quat Identity;

	public:

		 Quat() { }
		 Quat(float InX, float InY, float InZ, float InW);

		 Quat(const Quat& Q);

		
		explicit Quat(const Matrix& M);

		
		explicit Quat(const Rotator& R);

		/**
		 * Creates and initializes a new quaternion from the a rotation around the given axis.
		 *
		 * @param Axis assumed to be a normalized vector
		 * @param Angle angle to rotate above the given axis (in radians)
		 */
		Quat(Vector Axis, float AngleRad);

	public:



		
		 Quat operator+(const Quat& Q) const;

		
		 Quat operator+=(const Quat& Q);

		
		 Quat operator-(const Quat& Q) const;

		 bool Equals(const Quat& Q, float Tolerance = KINDA_SMALL_NUMBER) const;

	
		 bool IsIdentity(float Tolerance = SMALL_NUMBER) const;

		 Quat operator-=(const Quat& Q);

		
		 Quat operator*(const Quat& Q) const;

	
		 Quat operator*=(const Quat& Q);

	
		Vector operator*(const Vector& V) const;

	
		Matrix operator*(const Matrix& M) const;

		 Quat operator*=(const float Scale);

	
		 Quat operator*(const float Scale) const;

		
		 Quat operator/=(const float Scale);

	
		 Quat operator/(const float Scale) const;

	
		bool operator==(const Quat& Q) const;

		bool operator!=(const Quat& Q) const;

		float operator|(const Quat& Q) const;

	public:

	
		static  Quat MakeFromEuler(const Vector& Euler);

		/** Convert a Quaternion into floating-point Euler angles (in degrees). */
		 Vector Euler() const;

		/**
		 * Normalize this quaternion if it is large enough.
		 * If it is too small, returns an identity quaternion.
		 *
		 * @param Tolerance Minimum squared length of quaternion for normalization.
		 */
		 void Normalize(float Tolerance = SMALL_NUMBER);

		/**
		 * Get a normalized copy of this quaternion.
		 * If it is too small, returns an identity quaternion.
		 *
		 * @param Tolerance Minimum squared length of quaternion for normalization.
		 */
		 Quat GetNormalized(float Tolerance = SMALL_NUMBER) const;

		// Return true if this quaternion is normalized
		bool IsNormalized() const;

		/**
		 * Get the length of this quaternion.
		 *
		 * @return The length of this quaternion.
		 */
		 float Size() const;

		/**
		 * Get the length squared of this quaternion.
		 *
		 * @return The length of this quaternion.
		 */
		 float SizeSquared() const;


		/** Get the angle of this quaternion */
		 float GetAngle() const;

		/**
		 * get the axis and angle of rotation of this quaternion
		 *
		 * @param Axis{out] vector of axis of the quaternion
		 * @param Angle{out] angle of the quaternion
		 * @warning : assumes normalized quaternions.
		 */
		void ToAxisAndAngle(Vector& Axis, float& Angle) const;

		/**
		 * Get the swing and twist decomposition for a specified axis
		 *
		 * @param InTwistAxis Axis to use for decomposition
		 * @param OutSwing swing component quaternion
		 * @param OutTwist Twist component quaternion
		 * @warning assumes normalised quaternion and twist axis
		 */
		 void ToSwingTwist(const Vector& InTwistAxis, Quat& OutSwing, Quat& OutTwist) const;

		/**
		 * Rotate a vector by this quaternion.
		 *
		 * @param V the vector to be rotated
		 * @return vector after rotation
		 */
		Vector RotateVector(Vector V) const;

		/**
		 * Rotate a vector by the inverse of this quaternion.
		 *
		 * @param V the vector to be rotated
		 * @return vector after rotation by the inverse of this quaternion.
		 */
		Vector UnrotateVector(Vector V) const;

		/**
		 * @return quaternion with W=0 and V=theta*v.
		 */
		 Quat Log() const;

		/**
		 * @note Exp should really only be used after Log.
		 * Assumes a quaternion with W=0 and V=theta*v (where |v| = 1).
		 * Exp(q) = (sin(theta)*v, cos(theta))
		 */
		 Quat Exp() const;

		/**
		 * @return inverse of this quaternion
		 */
		 Quat Inverse() const;

		/**
		 * Enforce that the delta between this Quaternion and another one represents
		 * the shortest possible rotation angle
		 */
		void EnforceShortestArcWith(const Quat& OtherQuat);

		/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
		 Vector GetAxisX() const;

		/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
		 Vector GetAxisY() const;

		/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
		 Vector GetAxisZ() const;

		/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
		 Vector GetForwardVector() const;

		/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
		 Vector GetRightVector() const;

		/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
		 Vector GetUpVector() const;

		/** Convert a rotation into a unit vector facing in its direction. Equivalent to GetForwardVector(). */
		 Vector Vector() const;

		/** Get the FRotator representation of this Quaternion. */
		 Rotator Rotator() const;

		/**
		 * Get the axis of rotation of the Quaternion.
		 * This is the axis around which rotation occurs to transform the canonical coordinate system to the target orientation.
		 * For the identity Quaternion which has no such rotation, Vector(1,0,0) is returned.
		 */
		 oocd::Vector GetRotationAxis() const;

		/** Find the angular distance between two rotation quaternions (in radians) */
		 float AngularDistance(const Quat& Q) const;

		
		/**
		 * Utility to check if there are any non-finite values (NaN or Inf) in this Quat.
		 *
		 * @return true if there are any non-finite values in this Quaternion, otherwise false.
		 */
		bool ContainsNaN() const;

	

	public:

		/**
		 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
		 */
		static  Quat FindBetween(const  oocd::Vector& Vector1, const  oocd::Vector& Vector2)
		{
			return FindBetweenVectors(Vector1, Vector2);
		}

		/**
		 * Generates the 'smallest' (geodesic) rotation between two normals (assumed to be unit length).
		 */
		static  Quat FindBetweenNormals(const  oocd::Vector& Normal1, const  oocd::Vector& Normal2);

		/**
		 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
		 */
		static  Quat FindBetweenVectors(const  oocd::Vector& Vector1, const  oocd::Vector& Vector2);

		
		static  float Error(const Quat& Q1, const Quat& Q2);

		//自动规范化出错。
		static  float ErrorAutoNormalize(const Quat& A, const Quat& B);

		//快速线性四元数插值。
		static  Quat FastLerp(const Quat& A, const Quat& B, const float Alpha);

		//双线性四元数插值。
		static  Quat FastBilerp(const Quat& P00, const Quat& P10, const Quat& P01, const Quat& P11, float FracX, float FracY);

		/**球面插值。 将正确对齐。 结果不标准话。*/
		static  Quat Slerp_NotNormalized(const Quat &Quat1, const Quat &Quat2, float Slerp);
		/**球面插值。 将正确对齐。 结果归一化。*/
		static  Quat Slerp(const Quat &Quat1, const Quat &Quat2, float Slerp)
		{
			return Slerp_NotNormalized(Quat1, Quat2, Slerp).GetNormalized();
		}

		//更简单的Slerp，不会检查“最短距离”等。

		static  Quat SlerpFullPath_NotNormalized(const Quat &quat1, const Quat &quat2, float Alpha);

		//更简单的Slerp，不会检查“最短距离”等。
		static  Quat SlerpFullPath(const Quat &quat1, const Quat &quat2, float Alpha)
		{
			return SlerpFullPath_NotNormalized(quat1, quat2, Alpha).GetNormalized();
		}

	
		static  Quat Squad(const Quat& quat1, const Quat& tang1, const Quat& quat2, const Quat& tang2, float Alpha);

		
		static  Quat SquadFullPath(const Quat& quat1, const Quat& tang1, const Quat& quat2, const Quat& tang2, float Alpha);

		//计算给定点之间的切线
		static  void CalcTangents(const Quat& PrevP, const Quat& P, const Quat& NextP, float Tension, Quat& OutTan);

	};
}