#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include"MathHelper.h"
namespace oocd
{
	class Vector4
	{
	public:

		float X;

		float Y;

		float Z;

		float W;

	public:
		static const Vector4 ZeroVector;
		static const Vector4 ZeroAndUnitVector;
		Vector4(const Vector& InVector, float InW = 1.0f);

		explicit Vector4(float InX = 0.0f, float InY = 0.0f, float InZ = 0.0f, float InW = 1.0f);

		explicit Vector4(Vector2D InXY, Vector2D InZW);

	public:

		float& operator[](int ComponentIndex);

		//取负
		Vector4 operator-() const;
		//+
		Vector4 operator+(const Vector4& V) const;

		//+=
		Vector4 operator+=(const Vector4& V);

		//-
		Vector4 operator-(const Vector4& V) const;

		//-=
		Vector4 operator-=(const Vector4& V);

		//*
		Vector4 operator*(float Scale) const;

		// /
		Vector4 operator/(float Scale) const;

		// /
		Vector4 operator/(const Vector4& V) const;

		// *
		Vector4 operator*(const Vector4& V) const;

		//*=
		Vector4 operator*=(const Vector4& V);

		//  /=
		Vector4 operator/=(const Vector4& V);

		// *=
		Vector4 operator*=(float S);

		//计算dot3
		friend  float Dot3(const Vector4& V1, const Vector4& V2)
		{
			return V1.X*V2.X + V1.Y*V2.Y + V1.Z*V2.Z;
		}

		//计算dot4
		friend  float Dot4(const Vector4& V1, const Vector4& V2)
		{
			return V1.X*V2.X + V1.Y*V2.Y + V1.Z*V2.Z + V1.W*V2.W;
		}

		//得到变换后的

		friend  Vector4 operator*(float Scale, const Vector4& V)
		{
			return V.operator*(Scale);
		}

		//是否相等
		bool operator==(const Vector4& V) const;

		//是否不等
		bool operator!=(const Vector4& V) const;

		// Cross product
		Vector4 operator^(const Vector4& V) const;

	public:

		//得到第几个
		float& Component(int Index);

		//得到引用
		const float& Component(int Index) const;

		//是否相等
		bool Equals(const Vector4& V, float Tolerance = KINDA_SMALL_NUMBER) const;

		//标准化长度
		bool IsUnit3(float LengthSquaredTolerance = KINDA_SMALL_NUMBER) const;

		//todo：FString ToString() const;

		//todo:bool InitFromString(const FString& InSourceString);

		//得到安全的标准化
		Vector4 GetSafeNormal(float Tolerance = SMALL_NUMBER) const;

		//得到标准化
		Vector4 GetUnsafeNormal3() const;

		//todo: FRotator ToOrientationRotator() const;

		//todo： FQuat ToOrientationQuat() const;

		//todo: FRotator Rotation() const;

		void Set(float InX, float InY, float InZ, float InW);

		//长度
		float Size3() const;

		//长度平方
		float SizeSquared3() const;

		//有w的
		float Size() const;

		//有w的长度平方
		float SizeSquared() const;

		bool ContainsNaN() const;

		//检查是否0作用
		bool IsNearlyZero3(float Tolerance = KINDA_SMALL_NUMBER) const;

		//寻找对称点
		Vector4 Reflect3(const Vector4& Normal) const;

		//寻找UV坐标
		void FindBestAxisVectors3(Vector4& Axis1, Vector4& Axis2) const;
	};
	template <typename T>
	Vector3<T>::Vector3(const Vector4 &p) :X(p.X), Y(p.Y), Z(p.Z) {};
}
