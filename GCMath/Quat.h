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

		Quat(oocd::Vector Axis, float AngleRad);

	public:

		Quat operator+(const Quat& Q) const;

		Quat operator+=(const Quat& Q);

		Quat operator-(const Quat& Q) const;

		bool Equals(const Quat& Q, float Tolerance = KINDA_SMALL_NUMBER) const;

		bool IsIdentity(float Tolerance = SMALL_NUMBER) const;

		Quat operator-=(const Quat& Q);

		Quat operator*(const Quat& Q) const;
		//todo
		Quat operator*=(const Quat& Q);

		oocd::Vector operator*(const oocd::Vector& V) const;

		Matrix operator*(const Matrix& M) const;

		Quat operator*=(const float Scale);

		Quat operator*(const float Scale) const;

		Quat operator/=(const float Scale);

		Quat operator/(const float Scale) const;

		bool operator==(const Quat& Q) const;

		bool operator!=(const Quat& Q) const;

		float operator|(const Quat& Q) const;

	public:

		static  Quat MakeFromEuler(const oocd::Vector& Euler);

		oocd::Vector Euler() const;

		void Normalize(float Tolerance = SMALL_NUMBER);

		Quat GetNormalized(float Tolerance = SMALL_NUMBER) const;

		bool IsNormalized() const;

		float Size() const;

		float SizeSquared() const;

		float GetAngle() const;

		void ToAxisAndAngle(oocd::Vector& Axis, float& Angle) const;

		void ToSwingTwist(const oocd::Vector& InTwistAxis, Quat& OutSwing, Quat& OutTwist) const;

		oocd::Vector RotateVector(oocd::Vector V) const;

		oocd::Vector UnrotateVector(oocd::Vector V) const;

		Quat Inverse() const;

		void EnforceShortestArcWith(const Quat& OtherQuat);

		/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
		oocd::Vector GetAxisX() const;

		/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
		oocd::Vector GetAxisY() const;

		/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
		oocd::Vector GetAxisZ() const;

		/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
		oocd::Vector GetForwardVector() const;

		/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
		oocd::Vector GetRightVector() const;

		/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
		oocd::Vector GetUpVector() const;

		oocd::Vector Vector() const;

		oocd::Rotator Rotator() const;

		oocd::Vector GetRotationAxis() const;

		float AngularDistance(const Quat& Q) const;

		bool ContainsNaN() const;

	public:

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

		static  void CalcTangents(const Quat& PrevP, const Quat& P, const Quat& NextP, float Tension, Quat& OutTan);
	};
}