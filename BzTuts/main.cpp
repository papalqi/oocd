#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
#define DEBUG_PS   // Uncomment this line to debug pixel shaders


#define  PYTHON_TEST 
#include "stdafx.h"
#include"Engine.h"
#include "TestMeshcpp.h"
#include"OdRegex.h"

#ifdef PYTHON_TEST 

#include "../Python/Python3.7/Python.h"
#pragma comment(lib,R"(python37_d.lib)")
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	//检测内存泄漏
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#ifdef PYTHON_TEST
	Py_SetPythonHome(LR"(C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python36_64)");
	Py_Initialize();    
	if (!Py_IsInitialized())
	{
		printf("python初始化失败");
		return -1;
	}
	PyRun_SimpleString(R"(print('hello, world'))");

	Py_Finalize();   

	return 0;
#else
	try
	{
		Engine theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		return theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
#endif
	
}