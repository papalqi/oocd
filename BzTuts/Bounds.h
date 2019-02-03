//#pragma once
//#include"Vector2.h"
//#include"Vector3.h"
//#include<stdlib.h>
//#include<math.h>
//using namespace std;
//
//namespace oocd
//{
//	template <typename T>
//	class Bounds2
//	{
//	public:
//
//		// Bounds2 Public Methods
//		Bounds2();
//		explicit Bounds2(const Point2<T> &p) : pMin(p), pMax(p) {}
//
//		Bounds2(const Point2<T> &p1, const Point2<T> &p2);
//		template <typename U>
//		explicit operator Bounds2<U>() const
//		{
//			return Bounds2<U>((Point2<U>)pMin, (Point2<U>)pMax);
//		}
//
//		Vector2<T> Diagonal() const { return pMax - pMin; }
//		T Area() const;
//		int MaximumExtent() const;
//		inline const Point2<T> &operator[](int i) const { return (i == 0) ? pMin : pMax; }
//		inline Point2<T> &operator[](int i) { return (i == 0) ? pMin : pMax; }
//		bool operator==(const Bounds2<T> &b) const { return b.pMin == pMin && b.pMax == pMax; }
//		bool operator!=(const Bounds2<T> &b) const { return b.pMin != pMin || b.pMax != pMax; }
//		Point2<T> Lerp(const Point2f &t) const;
//		Vector2<T> Offset(const Point2<T> &p) const;
//		void BoundingSphere(Point2<T> *c, float *rad) const;
//		friend std::ostream &operator<<(std::ostream &os, const Bounds2<T> &b)
//		{
//			os << "[ " << b.pMin << " - " << b.pMax << " ]";
//			return os;
//		}
//
//		// Bounds2 Public Data
//		Point2<T> pMin, pMax;
//	};
//
//	template <typename T>
//	void oocd::Bounds2<T>::BoundingSphere(Point2<T> *c, float *rad) const
//	{
//		*c = (pMin + pMax) / 2;
//		*rad = Inside(*c, *this) ? Distance(*c, pMax) : 0;
//	}
//
//	template <typename T>
//	Point2<T> oocd::Bounds2<T>::Lerp(const Point2f &t) const
//	{
//		return Point2<T>(pbrt::Lerp(t.x, pMin.x, pMax.x),
//			pbrt::Lerp(t.y, pMin.y, pMax.y));
//	}
//
//	template <typename T>
//	oocd::Vector2<T> oocd::Bounds2<T>::Offset(const Point2<T> &p) const
//	{
//		Vector2<T> o = p - pMin;
//		if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
//		if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
//		return o;
//	}
//
//	template <typename T>
//	int oocd::Bounds2<T>::MaximumExtent() const
//	{
//		Vector2<T> diag = Diagonal();
//		if (diag.x > diag.y)
//			return 0;
//		else
//			return 1;
//	}
//
//	template <typename T>
//	T oocd::Bounds2<T>::Area() const
//	{
//		Vector2<T> d = pMax - pMin;
//		return (d.x * d.y);
//	}
//
//	template <typename T>
//	oocd::Bounds2<T>::Bounds2()
//	{
//		T minNum = std::numeric_limits<T>::lowest();
//		T maxNum = std::numeric_limits<T>::max();
//		pMin = Point2<T>(maxNum, maxNum);
//		pMax = Point2<T>(minNum, minNum);
//	}
//
//	template <typename T>
//	oocd::Bounds2<T>::Bounds2(const Point2<T> &p1, const Point2<T> &p2)
//	{
//		pMin = Point2<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
//		pMax = Point2<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
//	}
//
//	template <typename T>
//	class Bounds3 {
//	public:
//
//		// Bounds3 Public Methods
//		Bounds3() {
//			T minNum = std::numeric_limits<T>::lowest();
//			T maxNum = std::numeric_limits<T>::max();
//			pMin = Point3<T>(maxNum, maxNum, maxNum);
//			pMax = Point3<T>(minNum, minNum, minNum);
//		}
//		explicit Bounds3(const Point3<T> &p) : pMin(p), pMax(p) {}
//		Bounds3(const Point3<T> &p1, const Point3<T> &p2)
//			: pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
//				std::min(p1.z, p2.z)),
//			pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
//				std::max(p1.z, p2.z)) {}
//		const Point3<T> &operator[](int i) const;
//		Point3<T> &operator[](int i);
//		bool operator==(const Bounds3<T> &b) const {
//			return b.pMin == pMin && b.pMax == pMax;
//		}
//		bool operator!=(const Bounds3<T> &b) const {
//			return b.pMin != pMin || b.pMax != pMax;
//		}
//		Point3<T> Corner(int corner) const {
//			DCHECK(corner >= 0 && corner < 8);
//			return Point3<T>((*this)[(corner & 1)].x,
//				(*this)[(corner & 2) ? 1 : 0].y,
//				(*this)[(corner & 4) ? 1 : 0].z);
//		}
//		Vector3<T> Diagonal() const { return pMax - pMin; }
//		T SurfaceArea() const {
//			Vector3<T> d = Diagonal();
//			return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
//		}
//		T Volume() const {
//			Vector3<T> d = Diagonal();
//			return d.x * d.y * d.z;
//		}
//		int MaximumExtent() const {
//			Vector3<T> d = Diagonal();
//			if (d.x > d.y && d.x > d.z)
//				return 0;
//			else if (d.y > d.z)
//				return 1;
//			else
//				return 2;
//		}
//		Point3<T> Lerp(const Point3f &t) const {
//			return Point3<T>(pbrt::Lerp(t.x, pMin.x, pMax.x),
//				pbrt::Lerp(t.y, pMin.y, pMax.y),
//				pbrt::Lerp(t.z, pMin.z, pMax.z));
//		}
//		Vector3<T> Offset(const Point3<T> &p) const {
//			Vector3<T> o = p - pMin;
//			if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
//			if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
//			if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
//			return o;
//		}
//		void BoundingSphere(Point3<T> *center, float *radius) const {
//			*center = (pMin + pMax) / 2;
//			*radius = Inside(*center, *this) ? Distance(*center, pMax) : 0;
//		}
//		template <typename U>
//		explicit operator Bounds3<U>() const {
//			return Bounds3<U>((Point3<U>)pMin, (Point3<U>)pMax);
//		}
//		bool IntersectP(const Ray &ray, float *hitt0 = nullptr,
//			float *hitt1 = nullptr) const;
//		inline bool IntersectP(const Ray &ray, const Vector3f &invDir,
//			const int dirIsNeg[3]) const;
//		friend std::ostream &operator<<(std::ostream &os, const Bounds3<T> &b) {
//			os << "[ " << b.pMin << " - " << b.pMax << " ]";
//			return os;
//		}
//
//		// Bounds3 Public Data
//		Point3<T> pMin, pMax;
//	};
//
//	typedef Bounds2<float> Bounds2f;
//	typedef Bounds2<int> Bounds2i;
//	typedef Bounds3<float> Bounds3f;
//	typedef Bounds3<int> Bounds3i;
//}