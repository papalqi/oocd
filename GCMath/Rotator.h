#pragma once
#include "MathHelper.h"
#include "Vector3.h"
namespace oocd
{
	class Quat;
	class Rotator
	{
	public:

		//绕右轴旋转 Y
		float Pitch;

		//绕Z轴旋转
		float Yaw;

		//绕X轴旋转
		float Roll;

		static  const Rotator ZeroRotator;
	public:
		Rotator() {};
		explicit  Rotator(float InF);
		Rotator(float InPitch, float InYaw, float InRoll);

		//explicit  Rotator(const FQuat& Quat);
		~Rotator() {};

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

		//检查是否几乎为0
		bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const;

		//是否是0
		bool IsZero() const;

		//是否相等
		bool Equals(const Rotator& R, float Tolerance = KINDA_SMALL_NUMBER) const;

		//增加
		Rotator Add(float DeltaPitch, float DeltaYaw, float DeltaRoll);

		//得到逆todo
		 //Rotator GetInverse() const;

		//todo
		Rotator GridSnap(const Rotator& RotGrid) const;

		Vector GetVector() const;

		Quat Quaternion() const;

		//欧拉
		Vector Euler() const;

		//旋转Vector
		Vector RotateVector(const Vector& V) const;

		//逆然后应用
		Vector UnrotateVector(const Vector& V) const;

		//获得旋转值
		Rotator Clamp() const;

		//得到标准旋转
		Rotator GetNormalized() const;

		//todo
		Rotator GetDenormalized() const;

		//得到特定轴的值
		float GetComponentForAxis(Axis tAxis) const;

		//设置特定轴的值
		void SetComponentForAxis(Axis tAxis, float Component);

		//标准化
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
