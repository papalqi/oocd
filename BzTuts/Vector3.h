#pragma once
#include<math.h>
#include<iostream>
using namespace std;
namespace oocd
{


	//typedef Vector3<Float> Vector3f;
	//typedef Vector3<int> Vector3i;

	template <typename T>
	class Vector3
	{
	public:
		T X, Y,Z;
	public:

		static  const Vector3<T> ZeroVector;
		static  const Vector3<T> UnitVector;

	public:
		Vector3() { X = Y =Z = 0; }
		Vector3(T xx, T yy,T zz) : X(xx), Y(yy),Z(zz) { }

	public:
		Vector3<T> operator+(const Vector3<T>& other)const;
		Vector3<T> operator-(const Vector3<T> &v) const;
		template <typename U>Vector3<T> operator*(U f) const
		{
			return Vector3<T>(X*f, Y*f,Z*f);
		}
		Vector3<T>  operator*(const Vector3<T>& V) const;
		template <typename U>Vector3<T> operator/(U f) const
		{
			return Vector3<T>(X / f, Y / f,Z*f);
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
		Vector3<T> &operator+=(const Vector3<T>& Z)const;

		//-=
		Vector3<T> &operator-=(const Vector3<T> &V);
		bool operator==(const Vector3<T> &V) const;

		//！=
		bool operator!=(const Vector3<T> &V) const;

		//*=
		template <typename U>Vector3<T> &operator*=(U f)
		{
			X *= f; Y *= f;
			return *this;
		}

		// /=
		template <typename U>Vector3<T> &operator/=(U f);
		Vector3<T> operator-() const { return Vector3(-X, -Y); }

		//得到引用
		T &operator[](int Index) { return ((Index == 0) ? X : Y); }

	public:
		Vector3<T> GetAbs() const;
		Vector3<T> GetSignVector() const;
		bool IsZero() const;
		void Normalize(float Tolerance = SMALL_NUMBER);
		Vector3<T> GetSafeNormal(float Tolerance = SMALL_NUMBER) const;
		T Size() const;
		T GetMax() const;
		bool Equals(const Vector3<T>& V, float Tolerance = KINDA_SMALL_NUMBER) const;
	public:
		static float CrossProduct(const Vector3<T>& A, const Vector3<T>& B);
		static float Distance(const Vector3<T>& V1, const  Vector3<T>& V2);
		static float DistSquared(const Vector3<T>& V1, const  Vector3<T>& V2);
		static float DotProduct(const Vector3<T>& A, const Vector3<T>& B);
	};



	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::GetAbs() const
	{
		return Vector3(abs(X), abs(Y),abs(Z));

	}

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::GetSignVector() const
	{
		return Vector3<T>(Select(X, 1.0f, -1.0f), Select(Y, 1.0f, -1.0f), Select(Z, 1.0f, -1.0f));
	}

	template <typename T>
	bool oocd::Vector3<T>::IsZero() const
	{
		return X == 0 && Y == 0&&Z==0;
	}

	template <typename T>
	void oocd::Vector3<T>::Normalize(float Tolerance /*= SMALL_NUMBER*/)
	{
		const float SquareSum = X * X + Y * Y+Z*Z;
		if (SquareSum > Tolerance)
		{
			const float Scale = sqrt(SquareSum);
			X *= Scale;
			Y *= Scale;
			Z *= Scale;
			return;
		}
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::GetSafeNormal(float Tolerance /*= SMALL_NUMBER*/) const
	{
		const float SquareSum = X * X + Y * Y;
		if (SquareSum > Tolerance)
		{
			const float Scale = sqrt(SquareSum);
			return Vector3(X*Scale, Y*Scale,Z*Scale);
		}
		return ZeroVector;
	}

	template <typename T>
	T oocd::Vector3<T>::Size() const
	{
		return sqrt(X*X + Y * Y+Z*Z);
	}

	template <typename T>
	T oocd::Vector3<T>::GetMax() const
	{
		return max(X, Y,Z);
	}

	template <typename T>
	bool oocd::Vector3<T>::Equals(const Vector3<T>& V, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
	{
		return abs(X - V.X) <= Tolerance && abs(Y - V.Y) <= Tolerance && abs(Z - V.Z) <= Tolerance;
	}

	template <typename T>
	oocd::Vector3<T> oocd::Vector3<T>::operator*(const Vector3<T>& V) const
	{
		return Vector3<T>(X*V.X, Y*V.X,Z*V.Z);
	}



	template <typename T>
	bool Vector3<T>::operator!=(const Vector3<T> &V) const
	{
		return X != V.X || Y != V.Y||Z!=V.Z;
	}

	template <typename T>
	bool Vector3<T>::operator==(const Vector3<T> &V) const
	{
		return X == V.X && Y == V.Y; && Z == V.Z;
	}

	template <typename T>
	oocd::Vector3<T> & Vector3<T>::operator-=(const Vector3<T> &V)
	{
		X -= V.X; Y -= V.Y; Z -= V.Z;
		return *this;
	}

	template <typename T>
	oocd::Vector3<T> & Vector3<T>::operator+=(const Vector3<T>& V) const
	{
		X += V.X; Y += V.Y; Z+= V.Z;
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
		return X > Other.X && Y > Other.Y && Z> Other.Z;
	}

	template <typename T>
	bool Vector3<T>::operator<(const Vector3& Other) const
	{
		return X < Other.X && Y < Other.Y&&Z< Other.Z;
	}

	template <typename T>
	float Vector3<T>::DistSquared(const Vector3<T>& V1, const Vector3<T>& V2)
	{
		return pow(V2.X - V1.X, 2) + pow(V2.Y - V1.Y, 2)+ pow(V2.Z - V1.Z, 2)
	}

	template <typename T>
	float Vector3<T>::Distance(const Vector3<T>& V1, const Vector3<T>& V2)
	{
		return sqrt(DistSquared(V1, V2));
	}

	template <typename T>
	float Vector3<T>::CrossProduct(const Vector3<T>& A, const Vector3<T>& B)
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
		return Vector3<T>(X / V.X, Y / V.Y,Z/V.Z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T> &v) const
	{
		return Vector3<T>(X - v.X, Y - v.Y,Z-v.Z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const
	{
		return Vector3<T>(X + other.X, Y + other.Y,Z+other.Z);
	}


	template <typename T>
	inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
		os << "[ " << v.X << ", " << v.Y<<v.Z << " ]";
		return os;
	}

	//template <>
	//inline std::ostream &operator<<(std::ostream &os, const Vector3<float> &v) {
	//	os << StringPrintf("[ %f, %f ]", v.x, v.y);
	//	return os;
	//}


	typedef Vector3<float> Vector3f;
	typedef Vector3<int> Vector3i;
}
