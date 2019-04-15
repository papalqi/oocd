#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
#define DEBUG_PS   // Uncomment this line to debug pixel shaders



#include "stdafx.h"
#include"Engine.h"
#include "TestMeshcpp.h"
#include"OdRegex.h"




#ifdef PYTHON_TEST
extern "C" __declspec(dllexport) void  EnginMain(HINSTANCE hInstance)
{
	try
	{
		Engine theApp(hInstance);
		if (!theApp.Initialize())
		{

		}
		theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
	}

}

#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
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
	
}
#endif