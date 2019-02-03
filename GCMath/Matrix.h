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
		// Constructors.
		Matrix();
		~Matrix();
		//此函数实现在Plane文件中，对应的每一行的数值
		Matrix(const Plane& InX, const Plane& InY, const Plane& InZ, const Plane& InW);
		//对应的是每一行的数值，右下齐次为1
		Matrix(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW);

		// 初始化为标准矩阵
		inline void SetIdentity();
		//两个矩阵的乘法
		Matrix operator* (const Matrix& Other) const;
		//矩阵自乘
		void operator*=(const Matrix& Other);
		//矩阵加法
		Matrix operator+ (const Matrix& Other) const;
		//矩阵自加
		void operator+=(const Matrix& Other);
		//这不是放大，而是每个元素的乘
		Matrix operator* (float Other) const;
		//这不是放大，而是每个元素的乘
		void operator*=(float Other);
		//是否相等
		inline bool operator==(const Matrix& Other) const;
		//是否相等

		inline bool Equals(const Matrix& Other, float Tolerance = KINDA_SMALL_NUMBER) const;

		//矩阵是否不相等
		inline bool operator!=(const Matrix& Other) const;

		//应用于V4,V4为行向量，在左
		Vector4 TransformVector4(const Vector4& V) const;

		//进行变换
		Vector4 TransformPosition(const Vector &V) const;

		//反转并变换
		Vector InverseTransformPosition(const Vector &V) const;

		//变换方向向量
		Vector4 TransformVector(const Vector& V) const;

		// 颠倒矩阵.
		Matrix GetTransposed() const;

		//得到行列式

		inline float Determinant() const;

		//得到旋转的3x3行列式
		inline float RotDeterminant() const;

		//求逆
		inline Matrix InverseFast() const;

		//求逆
		inline Matrix Inverse() const;
		//转置伴随
		inline Matrix TransposeAdjoint() const;
		//删除任何的缩放
		inline void RemoveScaling(float Tolerance = SMALL_NUMBER);

		//得到/删除任何的缩放的矩阵
		inline Matrix GetMatrixWithoutScale(float Tolerance = SMALL_NUMBER) const;
		//删除任何缩放，返回带有缩放的vector
		inline Vector ExtractScaling(float Tolerance = SMALL_NUMBER);
		//得到带有缩放的vector
		inline Vector GetScaleVector(float Tolerance = SMALL_NUMBER) const;

		//删除平移
		inline Matrix RemoveTranslation() const;

		//增加评议
		inline Matrix ConcatTranslation(const Vector& Translation) const;

		//是否有NAN
		inline bool ContainsNaN() const;

		//通过Vector变换大小
		inline void ScaleTranslation(const Vector& Scale3D);

		//得到任意轴的最大scale
		inline float GetMaximumAxisScale() const;

		//应用scale
		inline Matrix ApplyScale(float Scale);

		//得到原点
		inline Vector GetOrigin() const;

		//
		inline Vector GetScaledAxis(Axis tAxis) const;

	//
		inline void GetScaledAxes(Vector &X, Vector &Y, Vector &Z) const;

		//得到单位轴
		inline Vector GetUnitAxis(Axis tAxis) const;

		//得到单位轴
		inline void GetUnitAxes(Vector &X, Vector &Y, Vector &Z) const;

		//设置轴
		inline void SetAxis(int i, const Vector& Axis);

		// 设置原点
		inline void SetOrigin(const Vector& NewOrigin);

		//设置轴
		inline void SetAxes(Vector* Axis0 = NULL, Vector* Axis1 = NULL, Vector* Axis2 = NULL, Vector* Origin = NULL);


		//得到某一列
		inline Vector GetColumn(int i) const;

		//todo: FRotator Rotator() const;

	
		//todo: FQuat ToQuat() const;

		//得到NearPlane
		bool GetFrustumNearPlane(Plane& OutPlane) const;

		//得到FarPlane
		bool GetFrustumFarPlane(Plane& OutPlane) const;

		//得到mLeftPlane
		bool GetFrustumLeftPlane(Plane& OutPlane) const;

		//得到RightPlane
		bool GetFrustumRightPlane(Plane& OutPlane) const;

		//得到topplane
		bool GetFrustumTopPlane(Plane& OutPlane) const;

		//得到BottomPlane
		bool GetFrustumBottomPlane(Plane& OutPlane) const;
	
		 // todo:FString ToString() const;

		/** Output ToString */
		void DebugPrint() const;



	
};

}
