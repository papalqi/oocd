

#include "stdafx.h"
#include"Pwindows.h"
#include"render.h"
#include"Engine.h"
#include "TestMeshcpp.h"
using namespace DirectX; 
extern bool Running;


int WINAPI WinMain(HINSTANCE hInstance,   
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)

{
	Engine mEngine;
	mEngine.EngineInit(hInstance, nShowCmd);
	// 前三个是位置，后4个是颜色
// a quad
	auto mesh1 = OCMesh:: CreateTestMesh();
	mEngine.LoadMesh(mesh1);
	auto mesh2 = OCMesh::CreateTestMesh();
	
	mesh2->MTransform.Position = XMFLOAT4(3.0f, 0.0f, 0.0f, 0.0f);
	mEngine.LoadMesh(mesh2);
	mEngine.EngineLoop();


	return 0;
}