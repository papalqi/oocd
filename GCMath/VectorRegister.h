#pragma once
#include "Type.h"
#include "DirectxMath.h"
#include "VectorRegisterCommon.h"
#define SIMD_ALIGNMENT (16)
#define ZERO_ANIMWEIGHT_THRESH (0.00001f)

namespace GlobalVectorConstants
{
	/*tatic const VectorRegister AnimWeightThreshold = MakeVectorRegister(ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH, ZERO_ANIMWEIGHT_THRESH);
	static const VectorRegister RotationSignificantThreshold = MakeVectorRegister(1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA, 1.0f - DELTA * DELTA);*/
}