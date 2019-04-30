#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
#define DEBUG_PS   // Uncomment this line to debug pixel shaders



#include "stdafx.h"
#include"Engine.h"
#include "TestMeshcpp.h"
#include"OdRegex.h"




#if PYTHON_TEST
extern "C" __declspec(dllexport) void  EnginMain(HWND bhw)
{
	try
	{
		Engine theApp((HINSTANCE)111);
#if QT_Engine_BIND
		if (!theApp.Initialize(bhw))
		{

		}
#else
		if (!theApp.Initialize())
		{

		}
#endif // QT_Engine_BIND

	
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
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	try
	{
		_CrtSetBreakAlloc(550911);
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