#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include"../GCMath/Matrix.h"
#include "../GCMath/Vector3.h"
using namespace DirectX;
struct Vertex {
	Vertex(float x, float y, float z, float r, float g, float b, float a);
	XMFLOAT3 pos;
	XMFLOAT4 color;
};