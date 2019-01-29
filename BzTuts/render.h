#pragma once
#include"stdafx.h"
#include <wrl.h>
using namespace DirectX;
using Microsoft::WRL::ComPtr;
const int frameBufferCount = 3;

// this is the structure of our constant buffer.
struct ConstantBuffer {
	XMFLOAT4 colorMultiplier;
};

class Render

{
public:

	//初始化
	bool InitD3D(int Width, int Height, HWND& hwnd, bool FullScreen, bool Running);

	void AddVertexsAndIndes(Vertex* vList, int vBufferSize, DWORD *iList, int LSize);
public:

	//基础的更新，tick
	void Update();
	void UpdatePipeline();

	void run();
	void Cleanup();

	void WaitForPreviousFrame();

	Render();
	~Render();
public:
	bool Running;
	int width;
	int height;
public:
	HANDLE fenceEvent;

private:

	//编译VertexShader
	bool compileVertexShader(D3D12_SHADER_BYTECODE& vertexShaderBytecode);

	//编译PixelShader
	bool compilePixelShader(D3D12_SHADER_BYTECODE &pixelShaderBytecode);

private:

	//PSO
	ComPtr<ID3D12PipelineState> pipelineStateObject; // pso containing a pipeline state

	ID3D12RootSignature* rootSignature; // root signature defines data shaders will access

	//屏幕视口
	D3D12_VIEWPORT viewport;

	// 超出这个区域会被裁剪
	D3D12_RECT scissorRect;
	void SetScissorRect();
	void SetViewport();

	//建立PSO和IL
	bool CreatePsoAndInputLayout(D3D12_SHADER_BYTECODE &vertexShaderBytecode, D3D12_SHADER_BYTECODE &pixelShaderBytecode);
private:

	// 命令队列
	ID3D12CommandQueue* commandQueue;

	//每一个buffer有一个Allocator
	ID3D12CommandAllocator* commandAllocator[frameBufferCount];

	//一个command list
	ID3D12GraphicsCommandList* commandList;

	//设置命令队列
	bool setCommandqueue();

private:

	//采样类型描述
	DXGI_SAMPLE_DESC sampleDesc = {};

	// direct3d device
	ID3D12Device* device;

	// 交换链
	IDXGISwapChain3* swapChain;

	//建立交换链
	void CreateSwapChain(HWND &hwnd, bool FullScreen, IDXGIFactory4* dxgiFactory);

	//设置适配器
	bool SetAdapter(IDXGIAdapter1*, IDXGIFactory4* dxgiFactory);

	//设置device
	bool SetDevice(IDXGIAdapter1* adapter);

private:

	//当command list执行时将锁住，
	ID3D12Fence* fence[frameBufferCount];

	//每一帧都会增加
	UINT64 fenceValue[frameBufferCount];

	//建立Fence，设置同步
	bool CreateFenceAndRootSignature();

private:

	ID3D12Resource* vertexBuffer;
	ID3D12Resource*indexBuffer;
	ID3D12Resource* depthStencilBuffer;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	void AddVertexs(Vertex* vList, int vBufferSize);
	void AddIndex(DWORD *iList, int LSize);

	void SetdepthStencil();

private:

	//render targets view 堆，有多少个缓存，里面就有几个这个
	ID3D12DescriptorHeap* rtvDescriptorHeap;

	//	depth
	ID3D12DescriptorHeap* dsDescriptorHeap;

	//视口数量
	ID3D12Resource* renderTargets[frameBufferCount];

	//当前的backbuffer
	int frameIndex;

	//RTV的size
	int rtvDescriptorSize;

	//建立Descriptor
	void CreateRtvDescriptor();
	private:
	//const buffer desc存储堆
	ID3D12DescriptorHeap* mainDescriptorHeap[frameBufferCount]; // this heap will store the descripor to our constant buffer
	//const buffer上传堆
	ID3D12Resource* constantBufferUploadHeap[frameBufferCount]; // this is the memory on the gpu where our constant buffer will be placed.

	ConstantBuffer cbColorMultiplierData; 
	UINT8* cbColorMultiplierGPUAddress[frameBufferCount]; // this is a pointer to the memory location we get when we map our constant buffer
};
