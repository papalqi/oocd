#include "Matrix.h"
#include "Plane.h"
using namespace oocd;
const Matrix Matrix::Identity(Plane(1, 0, 0, 0), Plane(0, 1, 0, 0),
	Plane(0, 0, 1, 0), Plane(0, 0, 0, 1));



oocd::Matrix::Matrix()
{

}

void oocd::Matrix::SetIdentity()
{
	M[0][0] = 1; M[0][1] = 0;  M[0][2] = 0;  M[0][3] = 0;
	M[1][0] = 0; M[1][1] = 1;  M[1][2] = 0;  M[1][3] = 0;
	M[2][0] = 0; M[2][1] = 0;  M[2][2] = 1;  M[2][3] = 0;
	M[3][0] = 0; M[3][1] = 0;  M[3][2] = 0;  M[3][3] = 1;
}

oocd::Matrix oocd::Matrix::operator*(const Matrix& Other) const
{
	Matrix Result;
	VectorMatrixMultiply(&Result, this, &Other);
	return Result;
}

bool oocd::Matrix::Equals(const Matrix& Other, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	for (int X = 0; X < 4; X++)
	{
		for (int Y = 0; Y < 4; Y++)
		{
			if (abs(M[X][Y] - Other.M[X][Y]) > Tolerance)
			{
				return false;
			}
		}
	}

	return true;
}

oocd::Vector4 oocd::Matrix::TransformVector4(const Vector4& V) const
{
	Vector4 Result;
	auto Tmp = V;

	Result[0] = Tmp[0] * M[0][0] + Tmp[1] * M[1][0] + Tmp[2] * M[2][0] + Tmp[3] * M[3][0];
	Result[1] = Tmp[0] * M[0][1] + Tmp[1] * M[1][1] + Tmp[2] * M[2][1] + Tmp[3] * M[3][1];
	Result[2] = Tmp[0] * M[0][2] + Tmp[1] * M[1][2] + Tmp[2] * M[2][2] + Tmp[3] * M[3][2];
	Result[3] = Tmp[0] * M[0][3] + Tmp[1] * M[1][3] + Tmp[2] * M[2][3] + Tmp[3] * M[3][3];

	return Result;
}

oocd::Vector4 oocd::Matrix::TransformPosition(const Vector &V) const
{
	return TransformVector4(Vector4(V.X, V.Y, V.Z, 1.0f));
}

oocd::Vector oocd::Matrix::InverseTransformPosition(const Vector &V) const
{
	Matrix InvSelf = this->InverseFast();
	return Vector(InvSelf.TransformPosition(V));
}

oocd::Vector4 oocd::Matrix::TransformVector(const Vector& V) const
{

	return TransformVector4(Vector4(V.X, V.Y, V.Z, 0.0f));

}

oocd::Matrix oocd::Matrix::GetTransposed() const
{
	Matrix	Result;

	Result.M[0][0] = M[0][0];
	Result.M[0][1] = M[1][0];
	Result.M[0][2] = M[2][0];
	Result.M[0][3] = M[3][0];

	Result.M[1][0] = M[0][1];
	Result.M[1][1] = M[1][1];
	Result.M[1][2] = M[2][1];
	Result.M[1][3] = M[3][1];

	Result.M[2][0] = M[0][2];
	Result.M[2][1] = M[1][2];
	Result.M[2][2] = M[2][2];
	Result.M[2][3] = M[3][2];

	Result.M[3][0] = M[0][3];
	Result.M[3][1] = M[1][3];
	Result.M[3][2] = M[2][3];
	Result.M[3][3] = M[3][3];

	return Result;
}

float oocd::Matrix::Determinant() const
{
	return	M[0][0] * (
		M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
		M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
		M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
		) -
		M[1][0] * (
			M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
			M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
			M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
			) +
		M[2][0] * (
			M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
			M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
			M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			) -
		M[3][0] * (
			M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
			M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
			M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			);
}

float oocd::Matrix::RotDeterminant() const
{
	return
		M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
}

oocd::Matrix oocd::Matrix::InverseFast() const
{
	Matrix Result;
	VectorMatrixInverse(&Result, this);
	return Result;
}

oocd::Matrix oocd::Matrix::Inverse() const
{
	Matrix Result;
	const float	Det = Determinant();

	if (Det == 0.0f)
	{
		Result = Matrix::Identity;
	}
	else
	{
		VectorMatrixInverse(&Result, this);
	}
}

oocd::Matrix oocd::Matrix::TransposeAdjoint() const
{
	Matrix TA;

	TA.M[0][0] = this->M[1][1] * this->M[2][2] - this->M[1][2] * this->M[2][1];
	TA.M[0][1] = this->M[1][2] * this->M[2][0] - this->M[1][0] * this->M[2][2];
	TA.M[0][2] = this->M[1][0] * this->M[2][1] - this->M[1][1] * this->M[2][0];
	TA.M[0][3] = 0.f;

	TA.M[1][0] = this->M[2][1] * this->M[0][2] - this->M[2][2] * this->M[0][1];
	TA.M[1][1] = this->M[2][2] * this->M[0][0] - this->M[2][0] * this->M[0][2];
	TA.M[1][2] = this->M[2][0] * this->M[0][1] - this->M[2][1] * this->M[0][0];
	TA.M[1][3] = 0.f;

	TA.M[2][0] = this->M[0][1] * this->M[1][2] - this->M[0][2] * this->M[1][1];
	TA.M[2][1] = this->M[0][2] * this->M[1][0] - this->M[0][0] * this->M[1][2];
	TA.M[2][2] = this->M[0][0] * this->M[1][1] - this->M[0][1] * this->M[1][0];
	TA.M[2][3] = 0.f;

	TA.M[3][0] = 0.f;
	TA.M[3][1] = 0.f;
	TA.M[3][2] = 0.f;
	TA.M[3][3] = 1.f;

	return TA;
}

void oocd::Matrix::RemoveScaling(float Tolerance /*= SMALL_NUMBER*/)
{
	const float SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const float SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const float SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);
	const float Scale0 = Select(SquareSum0 - Tolerance, Math::InvSqrt(SquareSum0), 1.0f);
	const float Scale1 = Select(SquareSum1 - Tolerance, Math::InvSqrt(SquareSum1), 1.0f);
	const float Scale2 = Select(SquareSum2 - Tolerance, Math::InvSqrt(SquareSum2), 1.0f);
	M[0][0] *= Scale0;
	M[0][1] *= Scale0;
	M[0][2] *= Scale0;
	M[1][0] *= Scale1;
	M[1][1] *= Scale1;
	M[1][2] *= Scale1;
	M[2][0] *= Scale2;
	M[2][1] *= Scale2;
	M[2][2] *= Scale2;
}

oocd::Matrix oocd::Matrix::GetMatrixWithoutScale(float Tolerance /*= SMALL_NUMBER*/) const
{
	Matrix Result = *this;
	Result.RemoveScaling(Tolerance);
	return Result;
}

oocd::Vector oocd::Matrix::ExtractScaling(float Tolerance /*= SMALL_NUMBER*/)
{
	Vector Scale3D(0, 0, 0);

	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	const float SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const float SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const float SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);

	if (SquareSum0 > Tolerance)
	{
		float Scale0 = Math::Sqrt(SquareSum0);
		Scale3D[0] = Scale0;
		float InvScale0 = 1.f / Scale0;
		M[0][0] *= InvScale0;
		M[0][1] *= InvScale0;
		M[0][2] *= InvScale0;
	}
	else
	{
		Scale3D[0] = 0;
	}

	if (SquareSum1 > Tolerance)
	{
		float Scale1 = Math::Sqrt(SquareSum1);
		Scale3D[1] = Scale1;
		float InvScale1 = 1.f / Scale1;
		M[1][0] *= InvScale1;
		M[1][1] *= InvScale1;
		M[1][2] *= InvScale1;
	}
	else
	{
		Scale3D[1] = 0;
	}

	if (SquareSum2 > Tolerance)
	{
		float Scale2 = Math::Sqrt(SquareSum2);
		Scale3D[2] = Scale2;
		float InvScale2 = 1.f / Scale2;
		M[2][0] *= InvScale2;
		M[2][1] *= InvScale2;
		M[2][2] *= InvScale2;
	}
	else
	{
		Scale3D[2] = 0;
	}

	return Scale3D;
}

oocd::Vector oocd::Matrix::GetScaleVector(float Tolerance /*= SMALL_NUMBER*/) const
{
	Vector Scale3D(1, 1, 1);

	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	for (int i = 0; i < 3; i++)
	{
		const float SquareSum = (M[i][0] * M[i][0]) + (M[i][1] * M[i][1]) + (M[i][2] * M[i][2]);
		if (SquareSum > Tolerance)
		{
			Scale3D[i] = Math::Sqrt(SquareSum);
		}
		else
		{
			Scale3D[i] = 0.f;
		}
	}

	return Scale3D;
}

oocd::Matrix oocd::Matrix::RemoveTranslation() const
{
	Matrix Result = *this;
	Result.M[3][0] = 0.0f;
	Result.M[3][1] = 0.0f;
	Result.M[3][2] = 0.0f;
	return Result;
}

oocd::Matrix oocd::Matrix::ConcatTranslation(const Vector& Translation) const
{
	Matrix Result;

	float* __restrict Dest = &Result.M[0][0];
	const float* __restrict Src = &M[0][0];
	const float* __restrict Trans = &Translation.X;

	Dest[0] = Src[0];
	Dest[1] = Src[1];
	Dest[2] = Src[2];
	Dest[3] = Src[3];
	Dest[4] = Src[4];
	Dest[5] = Src[5];
	Dest[6] = Src[6];
	Dest[7] = Src[7];
	Dest[8] = Src[8];
	Dest[9] = Src[9];
	Dest[10] = Src[10];
	Dest[11] = Src[11];
	Dest[12] = Src[12] + Trans[0];
	Dest[13] = Src[13] + Trans[1];
	Dest[14] = Src[14] + Trans[2];
	Dest[15] = Src[15];

	return Result;
}

bool oocd::Matrix::ContainsNaN() const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (Math::IsFinite(M[i][j]))
			{
				return true;
			}
		}
	}

	return false;
}

void oocd::Matrix::ScaleTranslation(const Vector& Scale3D)
{
	M[3][0] *= Scale3D.X;
	M[3][1] *= Scale3D.Y;
	M[3][2] *= Scale3D.Z;
}

float oocd::Matrix::GetMaximumAxisScale() const
{
	const float MaxRowScaleSquared = Math::Max(
		GetScaledAxis(Axis::X).SizeSquared(),
		Math::Max(
			GetScaledAxis(Axis::Y).SizeSquared(),
			GetScaledAxis(Axis::Z).SizeSquared()
		)
	);
	return Math::Sqrt(MaxRowScaleSquared);
}

oocd::Matrix oocd::Matrix::ApplyScale(float Scale)
{
	Matrix ScaleMatrix(
		Plane(Scale, 0.0f, 0.0f, 0.0f),
		Plane(0.0f, Scale, 0.0f, 0.0f),
		Plane(0.0f, 0.0f, Scale, 0.0f),
		Plane(0.0f, 0.0f, 0.0f, 1.0f)
	);
	return ScaleMatrix * (*this);
}

oocd::Vector oocd::Matrix::GetOrigin() const
{
	return Vector(M[3][0], M[3][1], M[3][2]);

}

oocd::Vector oocd::Matrix::GetScaledAxis(Axis tAxis) const
{
	switch (tAxis)
	{
	case Axis::X:
		return Vector(M[0][0], M[0][1], M[0][2]);

	case Axis::Y:
		return Vector(M[1][0], M[1][1], M[1][2]);

	case Axis::Z:
		return Vector(M[2][0], M[2][1], M[2][2]);

	default:
	
		return Vector::ZeroVector;
	}
}

void oocd::Matrix::GetScaledAxes(Vector &X, Vector &Y, Vector &Z) const
{
	X.X = M[0][0]; X.Y = M[0][1]; X.Z = M[0][2];
	Y.X = M[1][0]; Y.Y = M[1][1]; Y.Z = M[1][2];
	Z.X = M[2][0]; Z.Y = M[2][1]; Z.Z = M[2][2];
}

oocd::Vector oocd::Matrix::GetUnitAxis(Axis tAxis) const
{
	return GetScaledAxis(tAxis).GetSafeNormal();
}

void oocd::Matrix::GetUnitAxes(Vector &X, Vector &Y, Vector &Z) const
{
	GetScaledAxes(X, Y, Z);
	X.Normalize();
	Y.Normalize();
	Z.Normalize();
}

void oocd::Matrix::SetAxis(int i, const Vector& Axis)
{
	M[i][0] = Axis.X;
	M[i][1] = Axis.Y;
	M[i][2] = Axis.Z;
}

void oocd::Matrix::SetOrigin(const Vector& NewOrigin)
{

	M[3][0] = NewOrigin.X;
	M[3][1] = NewOrigin.Y;
	M[3][2] = NewOrigin.Z;
}

void oocd::Matrix::SetAxes(Vector* Axis0 /*= NULL*/, Vector* Axis1 /*= NULL*/, Vector* Axis2 /*= NULL*/, Vector* Origin /*= NULL*/)
{
	if (Axis0 != NULL)
	{
		M[0][0] = Axis0->X;
		M[0][1] = Axis0->Y;
		M[0][2] = Axis0->Z;
	}
	if (Axis1 != NULL)
	{
		M[1][0] = Axis1->X;
		M[1][1] = Axis1->Y;
		M[1][2] = Axis1->Z;
	}
	if (Axis2 != NULL)
	{
		M[2][0] = Axis2->X;
		M[2][1] = Axis2->Y;
		M[2][2] = Axis2->Z;
	}
	if (Origin != NULL)
	{
		M[3][0] = Origin->X;
		M[3][1] = Origin->Y;
		M[3][2] = Origin->Z;
	}
}

oocd::Vector oocd::Matrix::GetColumn(int i) const
{
	return Vector(M[0][i], M[1][i], M[2][i]);

}

bool oocd::Matrix::GetFrustumNearPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][2],
		M[1][2],
		M[2][2],
		M[3][2],
		OutPlane
	);
}

bool oocd::Matrix::GetFrustumFarPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][2],
		M[1][3] - M[1][2],
		M[2][3] - M[2][2],
		M[3][3] - M[3][2],
		OutPlane
	);
}

bool oocd::Matrix::GetFrustumLeftPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] + M[0][0],
		M[1][3] + M[1][0],
		M[2][3] + M[2][0],
		M[3][3] + M[3][0],
		OutPlane
	);
}

bool oocd::Matrix::GetFrustumRightPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][0],
		M[1][3] - M[1][0],
		M[2][3] - M[2][0],
		M[3][3] - M[3][0],
		OutPlane
	);
}

bool oocd::Matrix::GetFrustumTopPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][1],
		M[1][3] - M[1][1],
		M[2][3] - M[2][1],
		M[3][3] - M[3][1],
		OutPlane
	);
}

bool oocd::Matrix::GetFrustumBottomPlane(Plane& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] + M[0][1],
		M[1][3] + M[1][1],
		M[2][3] + M[2][1],
		M[3][3] + M[3][1],
		OutPlane
	);
}

bool oocd::Matrix::operator!=(const Matrix& Other) const
{
	return !(*this == Other);

}

bool oocd::Matrix::operator==(const Matrix& Other) const
{
	for (int X = 0; X < 4; X++)
		for (int Y = 0; Y < 4; Y++)
		{
			if (M[X][Y] != Other.M[X][Y])
			{
				return false;
			}
		}

	return true;
}

void oocd::Matrix::operator*=(float Other)
{
	*this = *this*Other;

}

oocd::Matrix oocd::Matrix::operator+(const Matrix& Other) const
{
	Matrix ResultMat;

	for (int X = 0; X < 4; X++)
	{
		for (int Y = 0; Y < 4; Y++)
		{
			ResultMat.M[X][Y] = M[X][Y] + Other.M[X][Y];
		}
	}

	return ResultMat;
}

oocd::Matrix oocd::Matrix::operator*(float Other) const
{
	Matrix ResultMat;

	for (int X = 0; X < 4; X++)
	{
		for (int Y = 0; Y < 4; Y++)
		{
			ResultMat.M[X][Y] = M[X][Y] * Other;
		}
	}

	return ResultMat;
}

void oocd::Matrix::operator+=(const Matrix& Other)
{
	*this = *this + Other;
}

void oocd::Matrix::operator*=(const Matrix& Other)
{
	VectorMatrixMultiply(this, this, &Other);

}

oocd::Matrix::Matrix(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW)
{
	M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = 0.0f;
	M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = 0.0f;
	M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = 0.0f;
	M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = 1.0f;
}

