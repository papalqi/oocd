#pragma once

#include "EngineDefine.h"
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




#if PYTHON_TEST
extern "C"  __declspec(dllexport) void  EnginMain(HWND bhw);

#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd);

#endif

	
