#pragma once
#include "OCMath.h"
#include <vector>
#include"stdafx.h"
#include<memory>
using namespace std;

struct OCTranForm
{
	XMFLOAT4X4 WorldMat;//世界矩阵
	XMFLOAT4X4 RotMat; //旋转矩阵
	XMFLOAT4 Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); //位置
};
struct OCMesh
{
	//变换
	OCTranForm MTransform;

	//索引
	vector<DWORD>iList;

	//顶点
	vector<Vertex>vList;

	//得到默认的mesh
	static OCMesh* CreateTestMesh();
	//registered
	void RegistereForRender(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	//得到原始索引数组
	DWORD *GetIndex();

	//得到原始顶点数组
	Vertex* GetVertex();

	//顶点大小，单位是字节
	int vBufferSize;

	//索引大小，单位是字节
	int iBufferSize;
	ID3D12Resource* vertexBuffer;
	ID3D12Resource*indexBuffer;
	//自己的vertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	//自己的indexBufferView
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	//深度测试
	ID3D12DescriptorHeap* dsDescriptorHeap;
private:

	//开始的指针
	DWORD*BeginiList;

	//开始的指针
	Vertex*BeginVList;
};
