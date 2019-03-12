#pragma once

#include "render.h"
#include "Pwindows.h"
#include"ErrorException.h"

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);
class Engine
{

public:
	int EngineInit(HINSTANCE hInstance, int nShowCmd);
	void LoadMesh(shared_ptr<OCMesh> one);
	void StartLoop();
	void EngineLoop();
	static Engine* GetEngine();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:

	GameTimer mtimer;
	static Engine* mEngine;
	Render OnlyRender;
	Pwindow Onlywindow;
	bool Running=true;
public:
};

