#pragma once


#include "OCMath.h"
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <wrl.h>
#include "d3dx12.h"
#include"ErrorException.h"
#include <string>
#include"MathHelper.h"
// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
//#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }


//#define  PYTHON_TEST 

#ifdef PYTHON_TEST
extern "C"  __declspec(dllexport) void  EnginMain(HWND bhw);

#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd);

#endif

	
