#pragma once
#include "CoreMinimal.h"
namespace oocd
{
	class BaseComponent
	{
	public:
		Vector Position;
		Rotator Rotation;

		Vector GetComponentLocation() { return Position; }
		Rotator GetComponentRotation() { return Rotation; }
	};
}
