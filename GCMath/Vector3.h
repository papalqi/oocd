#pragma once
#include"MathHelper.h"
#include<iostream>
#include <DirectXMath.h>
using namespace std;

namespace oocd
{
	//typedef Vector3<Float> Vector3f;
	//typedef Vector3<int> Vector3i;
	class Vector4;
	template <typename T>
	class Vector3
	{
	public:
		T X, Y, Z;
	public:

		static  const Vector3<T> ZeroVector;
		static const Vector3<T>OneVector;

	public:
		DirectX::XMFLOAT3 GetXMFLOAT3() {
			DirectX::XMFLOAT3 out; out.x = X; out.y = Y;
			out.z = Z; return out;
		};
		Vector3() { X = Y = Z = 0; }
		Vector3(DirectX::XMFLOAT3 DXM) { X = DXM.x; Y = DXM.y; Z = DXM.z; }
		Vector3(T zz) { X = Y = Z = zz; }
		Vector3(T xx, T yy, T zz) : X(xx), Y(yy), Z(zz) { }
		Vector3(const Vector3<T> &p) :X(p.X), Y(p.Y), Z(p.Z) {};
		//explicit Vector3(const Vector2D &p) :X(p.X), Y(p.Y), Z(0) {};
		 Vector3(const Vector4 &p);
		//explicit Vector3<T>(const Vector4 &p) : X(p.X), Y(p.Y), Z(p.Z) {};
	public:
		Vector3<T> operator+(const Vector3<T>& other)const;
		Vector3<T> operator-(const Vector3<T> &v) const;
		template <typename T>Vector3<T> operator*(T f) const
		{
			return Vector3<T>(X*f, Y*f, Z*f);
		}
		Vector3<T>  operator*(const Vector3<T>& V) const;
		template <typename U>Vector3<T> operator/(U f) const
		{
			return Vector3<T>(X / f, Y / f, Z/f);
		}

		Vector3<T> operator*=(const Vector3<T>& V)
		{
			X = X * V.X;
			Y = Y * V.Y;
			Z = Z * V.Z;
			return *this;
		}

		Vector3<T>  operator/(const Vector3<T>& V) const;

		//dot product
		T operator|(const Vector3<T>& V) const;

		//cross product
		Vector3<T> operator^(const Vector3<T>& V) const;

		//所有的都小于
		bool operator<(const Vector3<T>& Other) const;

		//所有的都大于
		bool operator>(const Vector3<T>& Other) const;

		//所有的都小于等于
		bool operator<=(const Vector3<T>& Other) const;

		//所有的都大于等于
		bool operator>=(const Vector3<T>& Other) const;

		//+=
		Vector3<T> &operator+=(const Vector3<T>& Z);

		//-=
		Vector3<T> &operator-=(const Vector3<T> &V);
		bool operator==(const Vector3<T> &V) const;

		//！=
		bool operator!=(const Vector3<T> &V) const;

		//*=
		Vector3<T> &operator*=(T f)
		{
			X *= f; Y *= f;
			return *this;
		}

		// /=
		template <typename T>Vector3<T> &operator/=(T f);
		Vector3<T> operator-() const { return Vector3<T>(-X, -Y,-Z); }

		//得到引用
		T &operator[](int Index) {
			if (Index == 0)
			{
				return X;
			}
			else if (Index == 1)
			{
				return Y;
			}
			else
			{
				return Z;
			}
		}

	public:
		Vector3<T> GetAbs() const;
		Vector3<T> GetSignVector() const;
		bool IsZero() const;
		bool Normalize(float Tolerance = SMALL_NUMBER);
		Vector3<T> GetSafeNormal(float Tolerance = SMALL_NUMBER) const;
		T Size() const;
		T SizeSquared()const;
		T GetMax() const;
		bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const;
		bool Equals(const Vector3<T>& V, float Tolerance = KINDA_SMALL_NUMBER) const;
	public:
		static Vector3<T> CrossProduct(const Vector3<T>& A, const Vector3<T>& B);
		static float Distance(const Vector3<T>& V1, const  Vector3<T>& V2);
		static float DistSquared(const Vector3<T>& V1, const  Vector3<T>& V2);
		static float DotProduct(const Vector3<T>& A, const Vector3<T>& B);
		static  Vector3<T> Vector3dMin(Vector3<T> V1, Vector3<T> V2);
		static  Vector3<T> Vector3dMax(Vector3<T> V1, Vector3<T> V2);
	};

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::Vector3dMax(Vector3<T> V1, Vector3<T> V2)
	{
		Vector3<T> Result;
		Result.X = (V1.X > V2.X) ? V1.X : V2.X;
		Result.Y = (V1.Y > V2.Y) ? V1.Y : V2.Y;
		Result.Z = (V1.Z > V2.Z) ? V1.Z : V2.Z;

		return Result;
	}

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::Vector3dMin(Vector3<T> V1, Vector3<T> V2)
	{
		Vector3<T> Result;
		Result.X = (V1.X < V2.X) ? V1.X : V2.X;
		Result.Y = (V1.Y < V2.Y) ? V1.Y : V2.Y;
		Result.Z = (V1.Z < V2.Z) ? V1.Z : V2.Z;
		
		return Result;
	}

	 template <typename T>
		const Vector3<T> oocd::Vector3<T>::ZeroVector(0.0f, 0.0f, 0.0f);
		
		template <typename T>
		const Vector3<T> oocd::Vector3<T>::OneVector(1.0f, 1.0f, 1.0f);
	

	template <typename T>
	bool oocd::Vector3<T>::IsNearlyZero(float Tolerance /*= KINDA_SMALL_NUMBER*/) const
	{
		return
			Math::Abs(X) <= Tolerance
			&& Math::Abs(Y) <= Tolerance
			&& Math::Abs(Z) <= Tolerance;
	}

	//Vector inline functions
	template <typename T>
	Vector3<T> operator*(float Scale, const Vector3<T>& V)
	{
		return V.operator*(Scale);
	}

	template <typename T>
	T oocd::Vector3<T>::SizeSquared() const
	{
		return X * X + Y * Y + Z * Z;
	}

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::GetAbs() const
	{
		return Vector3(abs(X), abs(Y), abs(Z));
	}

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::GetSignVector() const
	{
		return Vector3<T>(Select(X, 1.0f, -1.0f), Select(Y, 1.0f, -1.0f), Select(Z, 1.0f, -1.0f));
	}

	template <typename T>
	bool oocd::Vector3<T>::IsZero() const
	{
		return X == 0 && Y == 0 && Z == 0;
	}

	template <typename T>
	bool oocd::Vector3<T>::Normalize(float Tolerance /*= SMALL_NUMBER*/)
	{
		const float SquareSum = X * X + Y * Y + Z * Z;
		if (SquareSum > Tolerance)
		{
			const float Scale = Math::InvSqrt(SquareSum);
			X *= Scale; Y *= Scale; Z *= Scale;
			return true;
		}
		return false;
	}

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::GetSafeNormal(float Tolerance /*= SMALL_NUMBER*/) const
	{
		const float SquareSum = X * X + Y * Y + Z * Z;

		// Not sure if it's safe to add tolerance in there. Might introduce too many errors
		if (SquareSum == 1.f)
		{
			return *this;
		}
		else if (SquareSum < Tolerance)
		{
			return Vector3<T>::ZeroVector;
		}
		const float Scale = Math::InvSqrt(SquareSum);
		return Vector3<T>(X*Scale, Y*Scale, Z*Scale);
	}

	template <typename T>
	T oocd::Vector3<T>::Size() const
	{
		return sqrt(X*X + Y * Y + Z * Z);
	}

	template <typename T>
	T oocd::Vector3<T>::GetMax() const
	{
		return max(max(X, Y), Z);
	}

	template <typename T>
	bool oocd::Vector3<T>::Equals(const Vector3<T>& V, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
	{
		return abs(X - V.X) <= Tolerance && abs(Y - V.Y) <= Tolerance && abs(Z - V.Z) <= Tolerance;
	}

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::operator*(const Vector3<T>& V) const
	{
		return Vector3<T>(X*V.X, Y*V.X, Z*V.Z);
	}

	template <typename T>
	bool Vector3<T>::operator!=(const Vector3<T> &V) const
	{
		return X != V.X || Y != V.Y || Z != V.Z;
	}

	template <typename T>
	bool Vector3<T>::operator==(const Vector3<T> &V) const
	{
		return X == V.X && Y == V.Y && Z == V.Z;
	}

	template <typename T>
	oocd::Vector3<T> & Vector3<T>::operator-=(const Vector3<T> &V)
	{
		X -= V.X; Y -= V.Y; Z -= V.Z;
		return *this;
	}

	template <typename T>
	oocd::Vector3<T> & Vector3<T>::operator+=(const Vector3<T>& V) 
	{
		X += V.X; Y += V.Y; Z += V.Z;
		return *this;
	}

	template <typename T>
	bool Vector3<T>::operator>=(const Vector3& Other) const
	{
		return X >= Other.X && Y >= Other.Y&& Z >= Other.Z;
	}

	template <typename T>
	bool Vector3<T>::operator<=(const Vector3& Other) const
	{
		return X <= Other.X && Y <= Other.Y && Z <= Other.Z;
	}

	template <typename T>
	bool Vector3<T>::operator>(const Vector3& Other) const
	{
		return X > Other.X && Y > Other.Y && Z > Other.Z;
	}

	template <typename T>
	bool Vector3<T>::operator<(const Vector3& Other) const
	{
		return X < Other.X && Y < Other.Y&&Z < Other.Z;
	}

	template <typename T>
	float Vector3<T>::DistSquared(const Vector3<T>& V1, const Vector3<T>& V2)
	{
		return pow(V2.X - V1.X, 2) + pow(V2.Y - V1.Y, 2) + pow(V2.Z - V1.Z, 2);
	}

	template <typename T>
	float Vector3<T>::Distance(const Vector3<T>& V1, const Vector3<T>& V2)
	{
		return sqrt(DistSquared(V1, V2));
	}

	template <typename T>
	Vector3<T> Vector3<T>::CrossProduct(const Vector3<T>& A, const Vector3<T>& B)
	{
		return A ^ B;
	}

	template <typename T>
	float Vector3<T>::DotProduct(const Vector3<T>& A, const Vector3<T>& B)
	{
		return A | B;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator^(const Vector3& V) const
	{
		return Vector3<T>
			(
				Y * V.Z - Z * V.Y,
				Z * V.X - X * V.Z,
				X * V.Y - Y * V.X
				);
	}

	template <typename T>
	T Vector3<T>::operator|(const Vector3& V) const
	{
		return X * V.X + Y * V.Y + Z * V.Z;
	}

	template <typename T>
	oocd::Vector3<T> Vector3<T>::operator/(const Vector3<T>& V) const
	{
		return Vector3<T>(X / V.X, Y / V.Y, Z / V.Z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T> &v) const
	{
		return Vector3<T>(X - v.X, Y - v.Y, Z - v.Z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const
	{
		return Vector3<T>(X + other.X, Y + other.Y, Z + other.Z);
	}

	template <typename T>
	inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
		os << "[ " << v.X << ", " << v.Y << v.Z << " ]";
		return os;
	}

	typedef Vector3<float> Vector;
}
