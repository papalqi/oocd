﻿#pragma once
#include"stdafx.h"
#include "Matrix.h"
struct ConstantBufferPerObject
{
	oocd::Matrix wvpMat;
};


#define ConstantBufferPerObjectAlignedSize ((sizeof(ConstantBufferPerObject) + 255) & ~255)
#define  frameBufferCount 3
