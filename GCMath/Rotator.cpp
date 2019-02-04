#include "Rotator.h"
#include "RotationMatrix.h"

using namespace oocd;
const Rotator Rotator::ZeroRotator(0.f, 0.f, 0.f);

Rotator::Rotator(float InF) : Pitch(InF), Yaw(InF), Roll(InF)
{
}

Rotator Rotator::operator+(const Rotator& R) const
{
	return Rotator(Pitch + R.Pitch, Yaw + R.Yaw, Roll + R.Roll);
}

Rotator Rotator::operator-(const Rotator& R) const
{
	return Rotator(Pitch - R.Pitch, Yaw - R.Yaw, Roll - R.Roll);
}

Rotator Rotator::operator*(float Scale) const
{
	return Rotator(Pitch*Scale, Yaw*Scale, Roll*Scale);
}

bool Rotator::IsNearlyZero(float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	return
		Math::Abs<float>(NormalizeAxis(Pitch)) <= Tolerance
		&& Math::Abs<float>(NormalizeAxis(Yaw)) <= Tolerance
		&& Math::Abs<float>(NormalizeAxis(Roll)) <= Tolerance;
}

float Rotator::NormalizeAxis(float Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > 180.f)
	{
		// shift to (-180,180]
		Angle -= 360.f;
	}
	return Angle;
}

Rotator Rotator::operator-=(const Rotator& R)
{
	Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll;
	return *this;
}

Rotator Rotator::operator+=(const Rotator& R)
{
	Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll;
	return *this;
}

bool Rotator::operator!=(const Rotator& V) const
{
	return Pitch != V.Pitch || Yaw != V.Yaw || Roll != V.Roll;
}

bool Rotator::operator==(const Rotator& R) const
{
	return Pitch == R.Pitch && Yaw == R.Yaw && Roll == R.Roll;
}

Rotator Rotator::operator*=(float Scale)
{
	Pitch = Pitch * Scale; Yaw = Yaw * Scale; Roll = Roll * Scale;
	return *this;
}

Rotator::Rotator(float InPitch, float InYaw, float InRoll)
	: Pitch(InPitch), Yaw(InYaw), Roll(InRoll)
{
}

bool oocd::Rotator::IsZero() const
{
	return (ClampAxis(Pitch) == 0.f) && (ClampAxis(Yaw) == 0.f) && (ClampAxis(Roll) == 0.f);
}

bool oocd::Rotator::Equals(const Rotator& R, float Tolerance /*= KINDA_SMALL_NUMBER*/) const
{
	return (Math::Abs(NormalizeAxis(Pitch - R.Pitch)) <= Tolerance)
		&& (Math::Abs(NormalizeAxis(Yaw - R.Yaw)) <= Tolerance)
		&& (Math::Abs(NormalizeAxis(Roll - R.Roll)) <= Tolerance);
}

oocd::Rotator oocd::Rotator::Add(float DeltaPitch, float DeltaYaw, float DeltaRoll)
{
	Yaw += DeltaYaw;
	Pitch += DeltaPitch;
	Roll += DeltaRoll;
	return *this;
}

oocd::Rotator oocd::Rotator::GridSnap(const Rotator& RotGrid) const
{
	return Rotator
	(
		Math::GridSnap(Pitch, RotGrid.Pitch),
		Math::GridSnap(Yaw, RotGrid.Yaw),
		Math::GridSnap(Roll, RotGrid.Roll)
	);
}

oocd::Vector oocd::Rotator::GetVector() const
{
	float CP, SP, CY, SY;
	Math::SinCos(&SP, &CP, Math::DegreesToRadians(Pitch));
	Math::SinCos(&SY, &CY, Math::DegreesToRadians(Yaw));
	Vector V = Vector(CP*CY, CP*SY, SP);

	return V;
}



oocd::Vector oocd::Rotator::Euler() const
{
	return Vector(Roll, Pitch, Yaw);
}

oocd::Vector oocd::Rotator::UnrotateVector(const Vector& V) const
{
	return RotationMatrix(*this).GetTransposed().TransformVector(V);

}

oocd::Rotator oocd::Rotator::Clamp() const
{
	return Rotator(ClampAxis(Pitch), ClampAxis(Yaw), ClampAxis(Roll));

}

oocd::Rotator oocd::Rotator::GetNormalized() const
{
	Rotator Rot = *this;
	Rot.Normalize();
	return Rot;
}

oocd::Rotator oocd::Rotator::GetDenormalized() const
{
	Rotator Rot = *this;
	Rot.Pitch = ClampAxis(Rot.Pitch);
	Rot.Yaw = ClampAxis(Rot.Yaw);
	Rot.Roll = ClampAxis(Rot.Roll);
	return Rot;
}

float oocd::Rotator::GetComponentForAxis(Axis tAxis) const
{
	switch (tAxis)
	{
	case Axis::X:
		return Roll;
	case Axis::Y:
		return Pitch;
	case Axis::Z:
		return Yaw;
	default:
		return 0.f;
	}
}

void oocd::Rotator::SetComponentForAxis(Axis tAxis, float Component)
{
	switch (tAxis)
	{
	case Axis::X:
		Roll = Component;
		break;
	case Axis::Y:
		Pitch = Component;
		break;
	case Axis::Z:
		Yaw = Component;
		break;
	}
}

void oocd::Rotator::Normalize()
{
	Pitch = NormalizeAxis(Pitch);
	Yaw = NormalizeAxis(Yaw);
	Roll = NormalizeAxis(Roll);
}

void oocd::Rotator::GetWindingAndRemainder(Rotator& Winding, Rotator& Remainder) const
{
	//// YAW
	Remainder.Yaw = NormalizeAxis(Yaw);

	Winding.Yaw = Yaw - Remainder.Yaw;

	//// PITCH
	Remainder.Pitch = NormalizeAxis(Pitch);

	Winding.Pitch = Pitch - Remainder.Pitch;

	//// ROLL
	Remainder.Roll = NormalizeAxis(Roll);

	Winding.Roll = Roll - Remainder.Roll;
}

float oocd::Rotator::GetManhattanDistance(const Rotator & Rotator) const
{
	return Math::Abs<float>(Yaw - Rotator.Yaw) + Math::Abs<float>(Pitch - Rotator.Pitch) + Math::Abs<float>(Roll - Rotator.Roll);

}

oocd::Rotator oocd::Rotator::GetEquivalentRotator() const
{
	return Rotator(180.0f - Pitch, Yaw + 180.0f, Roll + 180.0f);

}

void oocd::Rotator::SetClosestToMe(Rotator& MakeClosest) const
{
	Rotator OtherChoice = MakeClosest.GetEquivalentRotator();
	float FirstDiff = GetManhattanDistance(MakeClosest);
	float SecondDiff = GetManhattanDistance(OtherChoice);
	if (SecondDiff < FirstDiff)
		MakeClosest = OtherChoice;
}

bool oocd::Rotator::ContainsNaN() const
{
	return (!Math::IsFinite(Pitch) ||
		!Math::IsFinite(Yaw) ||
		!Math::IsFinite(Roll));
}

float oocd::Rotator::ClampAxis(float Angle)
{
	// returns Angle in the range (-360,360)
	Angle = Math::Fmod(Angle, 360.f);

	if (Angle < 0.f)
	{
		// shift to [0,360) range
		Angle += 360.f;
	}

	return Angle;
}
