#include "Transform.h"

oocd::Transform::Transform() : MRotation(0.f, 0.f, 0.f, 1.f)
, Translation(0.f)
, Scale3D(Vector::OneVector)
{

}

oocd::Transform::Transform(const Transform& InTransform) :
	MRotation(InTransform.MRotation),
	Translation(InTransform.Translation),
	Scale3D(InTransform.Scale3D)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const oocd::Rotator& InRotation, const Vector& InTranslation, const Vector& InScale3D /*= Vector::OneVector*/) : MRotation(InRotation),
Translation(InTranslation),
Scale3D(InScale3D)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const Quat& InRotation, const Vector& InTranslation, const Vector& InScale3D /*= Vector::OneVector*/) : MRotation(InRotation),
Translation(InTranslation),
Scale3D(InScale3D)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const oocd::Rotator& InRotation) : MRotation(InRotation),
Translation(Vector::ZeroVector),
Scale3D(Vector::OneVector)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const Quat& InRotation) : MRotation(InRotation),
Translation(Vector::ZeroVector),
Scale3D(Vector::OneVector)
{
	DiagnosticCheckNaN_All();
}

oocd::Transform::Transform(const Vector& InTranslation) : MRotation(Quat::Identity),
Translation(InTranslation),
Scale3D(Vector::OneVector)
{
	DiagnosticCheckNaN_All();
}

