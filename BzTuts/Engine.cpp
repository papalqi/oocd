#include"Engine.h"
#include"ErrorException.h"
#include<windowsx.h>


Engine* Engine::mEngine = nullptr;


int Engine::EngineInit(HINSTANCE hInstance, int nShowCmd )
{
	mEngine = this;
	CHECK_AND_OUT(Onlywindow.InitializeWindow(hInstance, nShowCmd, false, WndProc), L"initialize error");
	CHECK_AND_OUT(OnlyRender.InitD3D(
		Onlywindow.GetWidth(),
		Onlywindow.GetHeight(),
		Onlywindow.Gethwnd(),
		Onlywindow.GetFullScreen(),
		Running),
		L"initialize direct3d 12 Error");
	OnlyRender.mhMainWnd = Onlywindow.Gethwnd();

	OnlyRender.mhMainWnd = Onlywindow.Gethwnd();
	return 0;
}

void Engine::LoadMesh(OCMesh *one)
{
	OnlyRender.LoadMesh(one);
}

void Engine::StartLoop()
{
	OnlyRender.CreateConstBuffer();
	EngineLoop();

	OnlyRender.WaitForPreviousFrame();

	CloseHandle(OnlyRender.fenceEvent);

	OnlyRender.Cleanup();
}




void Engine::EngineLoop()
{
	OnlyRender.CreateConstBuffer();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (Running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// run game code
			OnlyRender.Update(mtimer);
			OnlyRender.run();
		}
	}
}

Engine* Engine::GetEngine()
{
	return mEngine;

}

LRESULT Engine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				Running = false;
				DestroyWindow(hwnd);
			}
		}
		return 0;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mtimer.Stop();
		}
		else
		{
			mtimer.Start();
		}
		return 0;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnlyRender.OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnlyRender.OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnlyRender.OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_DESTROY: 
		Running = false;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)

{
	return Engine::GetEngine()->MsgProc(hwnd, msg, wParam, lParam);
}
