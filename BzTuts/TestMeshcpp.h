//#pragma once
//#include "OCMath.h"
//#include <vector>
//#include "Matrix.h"
//#include"stdafx.h"
//#include <wrl.h>
//#include<memory>
//using Microsoft::WRL::ComPtr;
//using namespace std;
//using namespace oocd;
//struct OCTranForm
//{
//	Matrix WorldMat;//世界矩阵
//	Matrix RotMat; //旋转矩阵
//	Vector Position = Vector::ZeroVector; //位置
//};
//struct OCMesh
//{
//	//变换
//	OCTranForm MTransform;
//
//	//索引
//	vector<DWORD>iList;
//
//	//顶点
//	vector<Vertex>vList;
//
//	//得到默认的mesh
//	static shared_ptr < OCMesh>CreateTestMesh();
//	//registered
//	void RegistereForRender(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
//	//得到原始索引数组
//	shared_ptr < DWORD >GetIndex();
//
//	//得到原始顶点数组
//	shared_ptr < Vertex> GetVertex();
//
//	//顶点大小，单位是字节
//	int vBufferSize;
//
//	//索引大小，单位是字节
//	int iBufferSize;
//	ComPtr < ID3D12Resource> vertexBuffer;
//	ComPtr < ID3D12Resource>indexBuffer;
//	//自己的vertexBufferView
//	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
//	//自己的indexBufferView
//	D3D12_INDEX_BUFFER_VIEW indexBufferView;
//	//深度测试
//	ComPtr < ID3D12DescriptorHeap> dsDescriptorHeap;
//private:
//
//	//开始的指针
//	shared_ptr<DWORD>BeginiList;
//
//	//开始的指针
//	shared_ptr <Vertex>BeginVList;
//};
