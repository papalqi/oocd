#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include"MathHelper.h"
/**
 * 4x4的矩阵
 *矩阵是行向量赋值，所以
 *
 *
 */
namespace  oocd
{
	class Matrix
	{
	public:
		float M[4][4];

		static const  Matrix Identity;
	public:
		//根据平移建立矩阵
		static 	Matrix			MatrixTranslation(float OffsetX, float OffsetY, float OffsetZ);
		//根据缩放建立矩阵
		static	Matrix			MatrixScale(float OffsetX, float OffsetY, float OffsetZ);
		//根据视角的位置建立视角矩阵
		static	Matrix			MatrixLookAtP(Vector EyePosition, Vector FocusPosition, Vector UpDirection);
		static	Matrix			MatrixLookAtD(Vector EyePosition, Vector EyeDirection, Vector UpDirection);
		//建立正交投影矩阵
		static	Matrix			MatrixOrtho(float l, float b, float n, float r, float t, float f);
	public:
		Matrix();
		Matrix(const Plane& InX, const Plane& InY, const Plane& InZ, const Plane& InW);
		Matrix(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW);
		
		Matrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
	public:
	
		bool 					ContainsNaN() const;
		bool 					Equals(const Matrix& Other, float Tolerance = KINDA_SMALL_NUMBER) const;
		bool 					operator!=(const Matrix& Other) const;
		bool 					GetFrustumFarPlane(Plane& OutPlane) const;
		bool 					GetFrustumNearPlane(Plane& OutPlane) const;
		bool 					GetFrustumRightPlane(Plane& OutPlane) const;
		bool					operator==(const Matrix& Other) const;
		bool					GetFrustumBottomPlane(Plane& OutPlane) const;
		bool					GetFrustumLeftPlane(Plane& OutPlane) const;
		bool					GetFrustumTopPlane(Plane& OutPlane) const;
		float					Determinant() const;
		float					GetMaximumAxisScale() const;
		float					RotDeterminant() const;
		Matrix 					ApplyScale(float Scale);
		Matrix 					ConcatTranslation(const Vector& Translation) const;
		Matrix 					GetMatrixWithoutScale(float Tolerance = SMALL_NUMBER) const;
		Matrix 					GetTransposed() const;
		Matrix 					Inverse() const;//计算逆矩阵
		Matrix 					InverseFast() const;
		Matrix 					operator* (const Matrix& Other) const;
		Matrix 					operator* (float Other) const;
		Matrix 					operator+ (const Matrix& Other) const;
		Matrix 					RemoveTranslation() const;
		Matrix 					TransposeAdjoint() const;

		Vector 					ExtractScaling(float Tolerance = SMALL_NUMBER);
		Vector 					GetColumn(int i) const;
		Vector 					GetOrigin() const;
		Vector 					GetScaledAxis(Axis tAxis) const;
		Vector 					GetScaleVector(float Tolerance = SMALL_NUMBER) const;
		Vector 					GetUnitAxis(Axis tAxis) const;
		Vector 					InverseTransformPosition(const Vector &V) const;
		Vector4 				TransformPosition(const Vector &V) const;
		Vector4 				TransformVector(const Vector& V) const;
		Vector4 				TransformVector4(const Vector4& V) const;
		void 					GetScaledAxes(Vector &X, Vector &Y, Vector &Z) const;
		void 					Mirror(Axis MirrorAxis, Axis FlipAxis);
		void 					operator*=(const Matrix& Other);
		void 					operator*=(float Other);
		void 					operator+=(const Matrix& Other);
		void 					RemoveScaling(float Tolerance = SMALL_NUMBER);
		void 					ScaleTranslation(const Vector& Scale3D);
		void 					SetAxes(Vector* Axis0 = NULL, Vector* Axis1 = NULL, Vector* Axis2 = NULL, Vector* Origin = NULL);
		void 					SetAxis(int i, const Vector& Axis);
		void 					SetIdentity();
		void 					SetOrigin(const Vector& NewOrigin);
		void					GetUnitAxes(Vector &X, Vector &Y, Vector &Z) const;
	};
}
