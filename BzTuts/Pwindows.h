#pragma once
#include"stdafx.h"
#include"render.h"
#include"Timer.h"
#define CHECK_AND_OUT(input,ErrorString)  if(input ==false) {MessageBox(0, ErrorString,L"Error", MB_OK);return 1;}

// callback function for windows messages

	// we will exit the program when this becomes false

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

class Pwindow
{
public:
	Pwindow();
	~Pwindow();

	// create a window
	bool InitializeWindow(HINSTANCE hInstance,
		int ShowWnd,
		bool fullscreen);

	// main application loop
	void mainloop();
	GameTimer mtimer;
	inline void SetWidth() { this->Width = Width; }
	inline void SetHeight() { this->Width = Height; }
	inline int GetWidth() { return Width; }
	inline int GetHeight() { return Height; }
	inline HWND& Gethwnd() { return hwnd; }
	inline bool GetFullScreen() { return FullScreen; }
	Render render;
private:

	//
	HWND hwnd = NULL;

	// windowd的名称
	LPCTSTR WindowName = L"papalqi";

	// 窗口名称
	LPCTSTR WindowTitle = L"papalqi";

	// 屏幕宽度
	int Width = 800;

	//屏幕高度
	int Height = 600;

	// 是否是全屏
	bool FullScreen = false;
};
