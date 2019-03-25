#pragma once
#include"stdafx.h"
#include "Matrix.h"
struct ConstantBufferPerObject
{
	oocd::Matrix wvpMat;
};

enum class RenderLayer : int
{
	Opaque = 0,
	Debug,
	Sky,
	Count
};

#define ConstantBufferPerObjectAlignedSize ((sizeof(ConstantBufferPerObject) + 255) & ~255)
#define  frameBufferCount 3

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif