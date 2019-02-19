#pragma once
#include "OCMath.h"
#include <vector>
#include<memory>
using namespace std;

struct OCTranForm
{

	XMFLOAT4X4 WorldMat;//世界矩阵
	XMFLOAT4X4 RotMat; //旋转矩阵
	XMFLOAT4 Position= XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) ; //位置
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
	static OCMesh CreateTestMesh();
	//得到原始索引数组
	DWORD *GetIndex();
	//得到原始顶点数组
	Vertex* GetVertex();
	//顶点大小，单位是字节
	int vBufferSize;
	//索引大小，单位是字节
	int iBufferSize;
private:
	//开始的指针
	DWORD*BeginiList;
	//开始的指针
	Vertex*BeginVList;
};




