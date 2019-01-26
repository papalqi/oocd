#include "stdafx.h"
#include"Pwindows.h"
#include"render.h"

using namespace DirectX; // we will be using the directxmath library

extern bool Running;

int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)

{
	//建立窗口
	Pwindow mwindow;

	//初始化窗口
	CHECK_AND_OUT(mwindow.InitializeWindow(hInstance, nShowCmd, false), L"initialize error");

	//初始化d3d
	CHECK_AND_OUT(mwindow.render.InitD3D(
		mwindow.GetWidth(),
		mwindow.GetHeight(),
		mwindow.Gethwnd(),
		mwindow.GetFullScreen(),
		Running),
		L"initialize direct3d 12 Error");


	// 前三个是位置，后4个是颜色
	Vertex vList[] = {
	{ -0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
	{  0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
	{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
	{  0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f }
	};


	int vBufferSize = sizeof(vList);
	mwindow.render.AddVertexs(vList, vBufferSize);

	DWORD iList[] = {
	0, 1, 2, // first triangle
	0, 3, 1 // second triangle
	};

	int iBufferSize = sizeof(iList);
	mwindow.render.AddIndex(iList, iBufferSize);
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