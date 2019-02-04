#pragma once
#include "Matrix.h"
#include "Rotator.h"
#include "Vector3.h"
namespace oocd
{
	class RotationMatrix : public Matrix
	{
	public:
		//旋转值和源点
		RotationMatrix(const Rotator& Rot, const Vector& Origin);
		/** Matrix factory. Return an Matrix so we don't have type conversion issues in expressions. */

		RotationMatrix(const Rotator& Rot)
			: RotationMatrix(Rot, Vector::ZeroVector)
		{ }
		static Matrix Make(const Rotator& Rot, const Vector& Origin)
		{
			return RotationMatrix(Rot, Origin);
		}
		static Matrix Make(Rotator const& Rot)
		{
			return RotationMatrix(Rot);
		}

		//static  Matrix Make(FQuat const& Rot);

		static  Matrix MakeFromX(Vector const& XAxis);

		static  Matrix MakeFromY(Vector const& YAxis);

		static  Matrix MakeFromZ(Vector const& ZAxis);

		static  Matrix MakeFromXY(Vector const& XAxis, Vector const& YAxis);

		static  Matrix MakeFromXZ(Vector const& XAxis, Vector const& ZAxis);

		static  Matrix MakeFromYX(Vector const& YAxis, Vector const& XAxis);

		static  Matrix MakeFromYZ(Vector const& YAxis, Vector const& ZAxis);

		static  Matrix MakeFromZX(Vector const& ZAxis, Vector const& XAxis);

		static  Matrix MakeFromZY(Vector const& ZAxis, Vector const& YAxis);
	};
}
