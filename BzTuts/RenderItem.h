#pragma once
#include "../GCMath/Matrix.h"
#include "Light.h"
#include "Mesh.h"
struct RenderItem
{
	RenderItem() = default;
	RenderItem(const RenderItem& rhs) = delete;

	Matrix World = Matrix::Identity;

	Matrix TexTransform = Matrix::Identity;

	//��������Ƿ������
	int NumFramesDirty = gNumFrameResources;

	//GPU�ĳ�������
	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};