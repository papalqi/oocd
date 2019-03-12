#pragma once


#include "OCMath.h"
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>

#include "d3dx12.h"
#include"ErrorException.h"
#include <string>

// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
//#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
