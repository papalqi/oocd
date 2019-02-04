#pragma once
#include "MathHelper.h"
#include "Vector3.h"
namespace oocd
{
	class Rotator
	{
	public:
		//��������ת Y
		float Pitch;
		//��Z����ת
		float Yaw;
		//��X����ת
		float Roll;

		static  const Rotator ZeroRotator;
	public:
		Rotator() {};
		explicit  Rotator(float InF);
		Rotator(float InPitch, float InYaw, float InRoll);
		//explicit  Rotator(const FQuat& Quat);
		~Rotator();

	public:

		Rotator operator+(const Rotator& R) const;

		Rotator operator-(const Rotator& R) const;

		Rotator operator*(float Scale) const;

		Rotator operator*=(float Scale);

		bool operator==(const Rotator& R) const;

		bool operator!=(const Rotator& V) const;

		Rotator operator+=(const Rotator& R);

		Rotator operator-=(const Rotator& R);

	public:

		// Functions.

		//����Ƿ񼸺�Ϊ0
		bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const;

		//�Ƿ���0
		bool IsZero() const;

		//�Ƿ����
		bool Equals(const Rotator& R, float Tolerance = KINDA_SMALL_NUMBER) const;

		//����
		Rotator Add(float DeltaPitch, float DeltaYaw, float DeltaRoll);

		//�õ���todo
		 //Rotator GetInverse() const;

		//todo
		Rotator GridSnap(const Rotator& RotGrid) const;

		Vector GetVector() const;

		//todo Quat Quaternion() const;

		//ŷ��
		Vector Euler() const;

		//��תVector
		Vector RotateVector(const Vector& V) const;

		//��Ȼ��Ӧ��
		Vector UnrotateVector(const Vector& V) const;

		//�����תֵ
		Rotator Clamp() const;

		//�õ���׼��ת
		Rotator GetNormalized() const;

		//todo
		Rotator GetDenormalized() const;

		//�õ��ض����ֵ
		float GetComponentForAxis(Axis tAxis) const;
		//�����ض����ֵ
		void SetComponentForAxis(Axis tAxis, float Component);

		//��׼��
		void Normalize();

		//todo
		void GetWindingAndRemainder(Rotator& Winding, Rotator& Remainder) const;

		//todo
		float GetManhattanDistance(const Rotator & Rotator) const;

		//todo
		Rotator GetEquivalentRotator() const;

		void SetClosestToMe(Rotator& MakeClosest) const;

		static float NormalizeAxis(float Angle);
		bool ContainsNaN() const;

	public:
		static float ClampAxis(float Angle);
	};
}
