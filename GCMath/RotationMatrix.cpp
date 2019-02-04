#include "RotationMatrix.h"

oocd::RotationMatrix::RotationMatrix(const Rotator& Rot, const Vector& Origin)
{
	float SP, SY, SR;
	float CP, CY, CR;
	Math::SinCos(&SP, &CP, Math::DegreesToRadians(Rot.Pitch));
	Math::SinCos(&SY, &CY, Math::DegreesToRadians(Rot.Yaw));
	Math::SinCos(&SR, &CR, Math::DegreesToRadians(Rot.Roll));

	M[0][0] = CP * CY;
	M[0][1] = CP * SY;
	M[0][2] = SP;
	M[0][3] = 0.f;

	M[1][0] = SR * SP * CY - CR * SY;
	M[1][1] = SR * SP * SY + CR * CY;
	M[1][2] = -SR * CP;
	M[1][3] = 0.f;

	M[2][0] = -(CR * SP * CY + SR * SY);
	M[2][1] = CY * SR - CR * SP * SY;
	M[2][2] = CR * CP;
	M[2][3] = 0.f;

	M[3][0] = Origin.X;
	M[3][1] = Origin.Y;
	M[3][2] = Origin.Z;
	M[3][3] = 1.f;
}

oocd::Matrix oocd::RotationMatrix::MakeFromX(Vector const& XAxis)
{
	Vector const NewX = XAxis.GetSafeNormal();

	// try to use up if possible
	Vector const UpVector = (Math::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);

	const Vector NewY = (UpVector ^ NewX).GetSafeNormal();
	const Vector NewZ = NewX ^ NewY;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

oocd::Matrix oocd::RotationMatrix::MakeFromY(Vector const& YAxis)
{
	Vector const NewY = YAxis.GetSafeNormal();

	// try to use up if possible
	Vector const UpVector = (Math::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);

	const Vector NewZ = (UpVector ^ NewY).GetSafeNormal();
	const Vector NewX = NewY ^ NewZ;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

oocd::Matrix oocd::RotationMatrix::MakeFromZ(Vector const& ZAxis)
{
	Vector const NewZ = ZAxis.GetSafeNormal();

	// try to use up if possible
	Vector const UpVector = (Math::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);

	const Vector NewX = (UpVector ^ NewZ).GetSafeNormal();
	const Vector NewY = NewZ ^ NewX;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

oocd::Matrix oocd::RotationMatrix::MakeFromXY(Vector const& XAxis, Vector const& YAxis)
{
	Vector NewX = XAxis.GetSafeNormal();
	Vector Norm = YAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewX | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewZ = (NewX ^ Norm).GetSafeNormal();
	const Vector NewY = NewZ ^ NewX;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

oocd::Matrix oocd::RotationMatrix::MakeFromXZ(Vector const& XAxis, Vector const& ZAxis)
{
	Vector const NewX = XAxis.GetSafeNormal();
	Vector Norm = ZAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewX | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewY = (Norm ^ NewX).GetSafeNormal();
	const Vector NewZ = NewX ^ NewY;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

oocd::Matrix oocd::RotationMatrix::MakeFromYX(Vector const& YAxis, Vector const& XAxis)
{
	Vector const NewY = YAxis.GetSafeNormal();
	Vector Norm = XAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewY | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewZ = (Norm ^ NewY).GetSafeNormal();
	const Vector NewX = NewY ^ NewZ;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

oocd::Matrix oocd::RotationMatrix::MakeFromYZ(Vector const& YAxis, Vector const& ZAxis)
{
	Vector const NewY = YAxis.GetSafeNormal();
	Vector Norm = ZAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewY | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewX = (NewY ^ Norm).GetSafeNormal();
	const Vector NewZ = NewX ^ NewY;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

oocd::Matrix oocd::RotationMatrix::MakeFromZX(Vector const& ZAxis, Vector const& XAxis)
{
	Vector const NewZ = ZAxis.GetSafeNormal();
	Vector Norm = XAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (Math::IsNearlyEqual(Math::Abs(NewZ | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewY = (NewZ ^ Norm).GetSafeNormal();
	const Vector NewX = NewY ^ NewZ;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}

oocd::Matrix oocd::RotationMatrix::MakeFromZY(Vector const& ZAxis, Vector const& YAxis)
{
	Vector const NewZ = ZAxis.GetSafeNormal();
	Vector Norm = YAxis.GetSafeNormal();

	//如果这两个几乎一样
	if (Math::IsNearlyEqual(Math::Abs(NewZ | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (Math::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? Vector(0, 0, 1.f) : Vector(1.f, 0, 0);
	}

	const Vector NewX = (Norm ^ NewZ).GetSafeNormal();
	const Vector NewY = NewZ ^ NewX;

	return Matrix(NewX, NewY, NewZ, Vector::ZeroVector);
}
oocd::Vector oocd::Rotator::RotateVector(const Vector& V) const
{
	return RotationMatrix(*this).TransformVector(V);
}