#pragma once
#include"stdafx.h"
#include <wrl.h>
#include "TestMeshcpp.h"
#include"Camera.h"
#include"Timer.h"
#include"RenderDefine.h"
using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Render

{
public:

	bool compilePixelShader(D3D12_SHADER_BYTECODE &pixelShaderBytecode);
	bool compileVertexShader(D3D12_SHADER_BYTECODE& vertexShaderBytecode);
	bool InitD3D(int Width, int Height, HWND& hwnd, bool FullScreen, bool Running);
	bool Running;

	Camera mCamera;
	ConstantBufferPerObject cbPerObject;
	D3D12_RECT scissorRect;
	D3D12_VIEWPORT viewport;
	DXGI_SAMPLE_DESC sampleDesc = {};
	HANDLE fenceEvent;
	HWND mhMainWnd;
	ID3D12CommandAllocator* commandAllocator[frameBufferCount];
	ID3D12CommandQueue* commandQueue;
	ID3D12DescriptorHeap* dsDescriptorHeap;
	ID3D12DescriptorHeap* rtvDescriptorHeap;
	ID3D12Device* device;
	ID3D12Fence* fence[frameBufferCount];
	ID3D12GraphicsCommandList* commandList;
	ID3D12PipelineState *pipelineStateObject;
	ID3D12Resource* constantBufferUploadHeaps[frameBufferCount];
	ID3D12Resource* depthStencilBuffer;
	ID3D12Resource* renderTargets[frameBufferCount];
	ID3D12RootSignature* rootSignature; // root signature defines data shaders will access
	IDXGISwapChain3* swapChain;
	int frameIndex;
	int height;
	int rtvDescriptorSize;
	int width;
	POINT mLastMousePos;
	UINT64 fenceValue[frameBufferCount];
	UINT8* cbvGPUAddress[frameBufferCount];
	vector<OCMesh*> renderMesh;
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	void Cleanup();
	void CreateRtvDescriptor();
	void CreateSwapChain(HWND &hwnd, bool FullScreen, IDXGIFactory4* dxgiFactory);
	void LoadMesh(OCMesh *one);
	void run();
	void Update(const GameTimer& gt);
	void UpdatePipeline();
	void WaitForPreviousFrame();
	void CreateConstBuffer();

private:
	void SetViewport();
	void SetScissorRect();
	void SetdepthStencil();
	void OnKeyboardInput(const GameTimer& gt);
	bool SetDevice(IDXGIAdapter1* adapter);
	bool CreatePsoAndInputLayout(D3D12_SHADER_BYTECODE &vertexShaderBytecode, D3D12_SHADER_BYTECODE &pixelShaderBytecode);
	bool CreateFenceAndRootSignature();
	bool SetAdapter(IDXGIAdapter1*, IDXGIFactory4* dxgiFactory);
	bool setCommandqueue();
};
