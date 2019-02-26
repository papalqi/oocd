#pragma once
#include"stdafx.h"

struct ConstantBufferPerObject
{
	XMFLOAT4X4 wvpMat;
};

struct ConstantBuffer {
	XMFLOAT4 colorMultiplier;
};

#define ConstantBufferPerObjectAlignedSize ((sizeof(ConstantBufferPerObject) + 255) & ~255)
#define  frameBufferCount 3
