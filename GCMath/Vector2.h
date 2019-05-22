#pragma once
#include"MathHelper.h"
#include<iostream>
using namespace std;

namespace oocd
{
	//typedef Vector3<Float> Vector3f;
	//typedef Vector3<int> Vector3i;
	template <typename T>
	class Vector2
	{
	public:
		T X, Y;
	public:

		static  const Vector2<T> ZeroVector;
		static  const Vector2<T> UnitVector;

	public:
		Vector2() { X = Y = 0; }
		Vector2(T xx, T yy) : X(xx), Y(yy) { }

		//explicit Vector2(const Vector3<T> &p) :X(p.X), Y(p.Y) {};
		 Vector2(const Vector2<T> &p) :X(p.X), Y(p.Y) {};
	public:
		Vector2<T> operator+(const Vector2<T>& other)const;
		Vector2<T> operator-(const Vector2<T> &v) const;
		template <typename T>Vector2<T> operator*(T f) const
		{
			return Vector2<T>(X*f, Y*f);
		}
		Vector2<T>  operator*(const Vector2<T>& V) const;
		template <typename U>Vector2<T> operator/(U f) const
		{
			return Vector2<T>(X / f, Y / f);
		}
		Vector2<T>  operator/(const Vector2<T>& V) const;

		//dot product
		T operator|(const Vector2& V) const;

		//cross product
		T operator^(const Vector2& V) const;

		//所有的都小于
		bool operator<(const Vector2<T>& Other) const;

		//所有的都大于
		bool operator>(const Vector2<T>& Other) const;

		//所有的都小于等于
		bool operator<=(const Vector2<T>& Other) const;

		//所有的都大于等于
		bool operator>=(const Vector2<T>& Other) const;

		//+=
		Vector2<T> &operator+=(const Vector2<T>& other)const;

		//-=
		Vector2<T> &operator-=(const Vector2<T> &V);
		bool operator==(const Vector2<T> &V) const;

		//！=
		bool operator!=(const Vector2<T> &V) const;

		//*=
		template <typename U>Vector2<T> &operator*=(U f)
		{
			X *= f; Y *= f;
			return *this;
		}

		// /=
		template <typename U>Vector2<T> &operator/=(U f);
		Vector2<T> operator-() const { return Vector2(-X, -Y); }

		//得到引用
		T &operator[](int Index) { return ((Index == 0) ? X : Y); }

	public:
		Vector2<T> GetAbs() const;
		Vector2<T> GetSignVector() const;
		bool IsZero() const;
		void Normalize(float Tolerance = SMALL_NUMBER);
		Vector2<T> GetSafeNormal(float Tolerance = SMALL_NUMBER) const;
		T Size() const;
		T GetMax() const;
		bool Equals(const Vector2<T>& V, float Tolerance = KINDA_SMALL_NUMBER) const;
	public:
		static float CrossProduct(const Vector2<T>& A, const Vector2<T>& B);
		static float Distance(const Vector2<T>& V1, const  Vector2<T>& V2);
		static float DistSquared(const Vector2<T>& V1, const  Vector2<T>& V2);
		static float DotProduct(const Vector2<T>& A, const Vector2<T>& B);
	};

	//Vector inline functions
	template <typename T>
	Vector2<T> operator*(float Scale, const Vector2<T>& V)
	{
		return V.operator*(Scale);
	}

	template <typename T>
	oocd::Vector2<T> oocd::Vector2<T>::GetAbs() const
	{
		return Vector2(abs(X), abs(Y));
	}

	template <typename T>
	oocd::Vector2<T> oocd::Vector2<T>::GetSignVector() const
	{
		return Vector2<T>(Select(X, 1.0f, -1.0f), Select(Y, 1.0f, -1.0f));
	}

	template <typename T>
	bool oocd::Vector2<T>::IsZero() const
	{
		return X == 0 && Y == 0;
	}

	template <typename T>
	void oocd::Vector2<T>::Normalize(float Tolerance /*= SMALL_NUMBER*/)
	{
		const float SquareSum = X * X + Y * Y;
		if (SquareSum > Tolerance)
		{
			const float Scale = sqrt(SquareSum);
			X *= Scale;
			Y *= Scale;
			return;
		}
		X = 0.0f;
		Y = 0.0f;
	}

	template <typename T>
	oocd::Vector2<T> oocd::Vector2<T>::GetSafeNormal(float Tolerance /*= SMALL_NUMBER*/) const
	{
		const float SquareSum = X * X + Y * Y;
		if (SquareSum > Tolerance)
		{
			const float Scale = sqrt(SquareSum);
			return Vector2(X*Scale, Y*Scale);
		}
		return ZeroVector;
	}

	template <typename T>
	T oocd::Vector2<T>::Size() const
	{
		return sqrt(X*X + Y * Y);
	}

	template <typename T>
	T oocd::Vector2<T>::GetMax() const
	{
		return max(X, Y);
	}

	template <typename T>
	bool oocd::Vector2<T>::Equals(const Vector2<T>& V, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
	{
		return abs(X - V.X) <= Tolerance && abs(Y - V.Y) <= Tolerance;
	}

	template <typename T>
	oocd::Vector2<T> oocd::Vector2<T>::operator*(const Vector2<T>& V) const
	{
		return Vector2<T>(X*V.X, Y*V.Y);
	}

	template <typename T>
	bool Vector2<T>::operator!=(const Vector2<T> &V) const
	{
		return X != V.X || Y != V.Y;
	}

	template <typename T>
	bool Vector2<T>::operator==(const Vector2<T> &V) const
	{
		return X == V.X && Y == V.Y;
	}

	template <typename T>
	oocd::Vector2<T> & Vector2<T>::operator-=(const Vector2<T> &V)
	{
		X -= V.X; Y -= V.Y;
		return *this;
	}

	template <typename T>
	oocd::Vector2<T> & Vector2<T>::operator+=(const Vector2<T>& V) const
	{
		X += V.X; Y += V.Y;
		return *this;
	}

	template <typename T>
	bool Vector2<T>::operator>=(const Vector2<T>& Other) const
	{
		return X >= Other.X && Y >= Other.Y;
	}

	template <typename T>
	bool Vector2<T>::operator<=(const Vector2<T>& Other) const
	{
		return X <= Other.X && Y <= Other.Y;
	}

	template <typename T>
	bool Vector2<T>::operator>(const Vector2<T>& Other) const
	{
		return X > Other.X && Y > Other.Y;
	}

	template <typename T>
	bool Vector2<T>::operator<(const Vector2& Other) const
	{
		return X < Other.X && Y < Other.Y;
	}

	template <typename T>
	float Vector2<T>::DistSquared(const Vector2<T>& V1, const Vector2<T>& V2)
	{
		return pow(V2.X - V1.X, 2) + pow(V2.Y - V1.Y, 2);
	}

	template <typename T>
	float Vector2<T>::Distance(const Vector2<T>& V1, const Vector2<T>& V2)
	{
		return sqrt(DistSquared(V1, V2));
	}

	template <typename T>
	float Vector2<T>::CrossProduct(const Vector2<T>& A, const Vector2<T>& B)
	{
		return A ^ B;
	}

	template <typename T>
	float Vector2<T>::DotProduct(const Vector2<T>& A, const Vector2<T>& B)
	{
		return A | B;
	}

	template <typename T>
	T Vector2<T>::operator^(const Vector2& V) const
	{
		return X * V.Y - Y * V.X;
	}

	template <typename T>
	T Vector2<T>::operator|(const Vector2& V) const
	{
		return X * V.X + Y * V.Y;
	}

	template <typename T>
	oocd::Vector2<T> Vector2<T>::operator/(const Vector2<T>& V) const
	{
		return Vector2<T>(X / V.X, Y / V.Y);
	}

	template <typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2<T> &v) const
	{
		return Vector2<T>(X - v.X, Y - v.Y);
	}

	template <typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
	{
		return Vector2<T>(X + other.X, Y + other.Y);
	}

	template <typename T>
	inline std::ostream &operator<<(std::ostream &os, const Vector2<T> &v) {
		os << "[ " << v.X << ", " << v.Y << " ]";
		return os;
	}

	typedef Vector2<float> Vector2D;
}
