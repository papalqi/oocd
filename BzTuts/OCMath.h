#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include"Matrix.h"
#include"RotationMatrix.h"
#include "Vector3.h"
#include"MathHelper.h"
using namespace DirectX;
struct Vertex {
	Vertex() {};
	Vertex(float x, float y, float z, float r, float g, float b, float a);
	XMFLOAT3 pos;
	XMFLOAT4 color;
};