#pragma once
#include"stdafx.h"
#include <wrl.h>
#include <Windows.h>
#include"RenderDefine.h"
#include "ETimer.h"
using namespace DirectX;
using namespace Microsoft;
using namespace Microsoft::WRL;


class EngineBase
{
protected:

	EngineBase(HINSTANCE hInstance);
	EngineBase(const EngineBase& rhs) = delete;//禁止使用
	EngineBase& operator=(const EngineBase& rhs) = delete;//禁止使用
	virtual ~EngineBase();

public:

	static EngineBase*				GetEngine();

	HINSTANCE						AppInst()const;
	HWND							MainWnd()const;
	float							AspectRatio()const;
	bool							Get4xMsaaState()const;
	void							Set4xMsaaState(bool value);
	
	int								Run();
	virtual bool					Initialize();
	virtual bool					Initialize(HWND Bwindows);
	virtual LRESULT					MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	string							GetFpsAndMspf();

protected:
	virtual void					CreateRtvAndDsvDescriptorHeaps();//建立存储区
	virtual void					OnResize();
	virtual void					Update(const GameTimer& gt) = 0;
	virtual void					Draw(const GameTimer& gt) = 0;

	virtual void					OnMouseDown(WPARAM btnState, int x, int y);
	virtual void					OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void					OnMouseMove(WPARAM btnState, int x, int y) { }

protected:

	bool							InitMainWindow();	//初始化窗口
	bool							InitMainWindow(HWND BWINDOWS);
	bool							InitDirect3D();		//初始化D3D
	void							CreateCommandObjects();//建立Command相关
	void							CreateSwapChain();	//建立交换链
	void							FlushCommandQueue();
	ID3D12Resource*					CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE		CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE		DepthStencilView()const;
	void							CalculateFrameStats();//计算帧率

	void							LogAdapters();
	void							LogAdapterOutputs(IDXGIAdapter* adapter);
	void							LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:
	static const int				SwapChainBufferCount = 2;  //双缓冲
	static EngineBase*				mEngine;
	bool							m4xMsaaState = false;
	bool							mEnginePaused = false;
	bool							mFullscreenState = false;
	bool							mMaximized = false;
	bool							mMinimized = false;
	bool							mResizing = false;
	ComPtr<ID3D12CommandAllocator>	mDirectCmdListAlloc;		//指令分配器
	ComPtr<ID3D12CommandQueue>		mCommandQueue;				//指令队列
	ComPtr<ID3D12DescriptorHeap>	mDsvHeap;//dsv存储区ShaderResourceView存放处
	ComPtr<ID3D12DescriptorHeap>	mRtvHeap;//rtv存储区renderTargetView存放处
	ComPtr<ID3D12Device>			md3dDevice;										//表示虚拟适配器; 它用于创建命令分配器，命令列表，命令队列，围栏，资源，管道状态对象，堆，根签名，采样器和许多资源视图
	ComPtr<ID3D12Fence>				mFence;											//表示fence，用于同步CPU和一个或多个GPU的对象。
	ComPtr<ID3D12GraphicsCommandList> mCommandList;				//指令列表
	ComPtr<ID3D12Resource>			mDepthStencilBuffer;							//深度 / 模板缓冲区描述符
	ComPtr<ID3D12Resource>			mSwapChainBuffer[SwapChainBufferCount];			//交换链资源描述符
	ComPtr<IDXGIFactory4>			mdxgiFactory;									//图形基础结构（DXGI）对象
	ComPtr<IDXGISwapChain>			mSwapChain;					//交换链 存储渲染数据。
	D3D_DRIVER_TYPE					md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	D3D12_RECT						mScissorRect;									//
	D3D12_VIEWPORT					mScreenViewport;								//后台缓冲区的视口
	DXGI_FORMAT						mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;//后台存储区格式
	DXGI_FORMAT						mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;//depth格式
	GameTimer						mTimer;
	HINSTANCE						mhWindowsInst = nullptr;
	HWND							mhMainWnd = nullptr;
	int								mClientHeight = 600;
	int								mClientWidth = 800;
	int								mCurrBackBuffer = 0; //跟踪当前的后台缓冲区索引

	std::wstring					mMainWndCaption = L"Papalqu";
	UINT							m4xMsaaQuality = 0;
	UINT							mCbvSrvUavDescriptorSize = 0;//cbv的句柄大小
	UINT							mDsvDescriptorSize = 0;//Dsv深度/模板视图的句柄大小
	UINT							mRtvDescriptorSize = 0;//rtv渲染目标视图的句柄大小
	UINT64							mCurrentFence = 0;
};
