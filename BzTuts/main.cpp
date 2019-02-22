

#include "stdafx.h"
#include"Pwindows.h"
#include"render.h"
#include "TestMeshcpp.h"
using namespace DirectX; 
extern bool Running;


int WINAPI WinMain(HINSTANCE hInstance,   
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)

{
	
	Pwindow mwindow;

	//todo:将要转换到engine
	CHECK_AND_OUT(mwindow.InitializeWindow(hInstance, nShowCmd, false), L"initialize error");

	//todo:将要转换到engine
	CHECK_AND_OUT(mwindow.render.InitD3D(
		mwindow.GetWidth(),
		mwindow.GetHeight(),
		mwindow.Gethwnd(),
		mwindow.GetFullScreen(),
		Running),
		L"initialize direct3d 12 Error");

	// 前三个是位置，后4个是颜色
// a quad
	auto mesh1 = OCMesh:: CreateTestMesh();
	mwindow.render.LoadMesh(mesh1);
	auto mesh2 = OCMesh::CreateTestMesh();
	
	mesh2->MTransform.Position = XMFLOAT4(3.0f, 0.0f, 0.0f, 0.0f);
	mwindow.render.LoadMesh(mesh2);

	mwindow.render.LoadMeshEnd();
	// start the main loop
	mwindow.mainloop();

	// we want to wait for the gpu to finish executing the command list before we start releasing everything
	mwindow.render.WaitForPreviousFrame();

	// close the fence event
	CloseHandle(mwindow.render.fenceEvent);

	// clean up everything
	mwindow.render.Cleanup();

	return 0;
}