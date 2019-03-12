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

	bool						 compilePixelShader(D3D12_SHADER_BYTECODE &pixelShaderBytecode);
	bool						 compileVertexShader(D3D12_SHADER_BYTECODE& vertexShaderBytecode);
	bool						 InitD3D(int Width, int Height, HWND& hwnd, bool FullScreen, bool Running);
	bool									Running;

	NCamera									mCamera;
	ConstantBufferPerObject					cbPerObject;
	D3D12_RECT								scissorRect;
	D3D12_VIEWPORT							viewport;
	DXGI_SAMPLE_DESC						sampleDesc = {};
	HANDLE									fenceEvent;
	HWND									mhMainWnd;
	ComPtr<ID3D12CommandAllocator>			commandAllocator[frameBufferCount];
	ComPtr<ID3D12CommandQueue>				commandQueue;
	ComPtr<ID3D12DescriptorHeap>			dsDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>			rtvDescriptorHeap;
	ComPtr<ID3D12Device>					device;
	ComPtr<ID3D12Fence>						fence[frameBufferCount];
	ComPtr<ID3D12GraphicsCommandList>		commandList;
	ComPtr<ID3D12PipelineState >			pipelineStateObject;
	ComPtr<ID3D12Resource>					constantBufferUploadHeaps[frameBufferCount];
	ComPtr<ID3D12Resource>					depthStencilBuffer;
	ComPtr<ID3D12Resource>					renderTargets[frameBufferCount];
	ComPtr<ID3D12RootSignature>				rootSignature; 
	ComPtr<IDXGISwapChain3>					swapChain;
	int										frameIndex;
	int										height;
	int										rtvDescriptorSize;
	int										width;
	POINT									mLastMousePos;
	UINT64									fenceValue[frameBufferCount];
	shared_ptr <UINT8>							cbvGPUAddress[frameBufferCount];
	vector<shared_ptr<OCMesh>>					renderMesh;
	virtual void				OnMouseDown(WPARAM btnState, int x, int y);
	virtual void				OnMouseMove(WPARAM btnState, int x, int y);
	virtual void				OnMouseUp(WPARAM btnState, int x, int y);
	void						Cleanup();
	void						CreateRtvDescriptor();
	void						CreateSwapChain(HWND &hwnd, bool FullScreen, IDXGIFactory4* dxgiFactory);
	void						LoadMesh(shared_ptr<OCMesh>one);
	void						run();
	void						Update(const GameTimer& gt);
	void						UpdatePipeline();
	void						WaitForPreviousFrame();
	void						CreateConstBuffer();

private:
	void						SetViewport();
	void						SetScissorRect();
	void						SetdepthStencil();
	void						OnKeyboardInput(const GameTimer& gt);
	bool						SetDevice(ComPtr < IDXGIAdapter1> );
	bool						CreatePsoAndInputLayout(D3D12_SHADER_BYTECODE &vertexShaderBytecode, D3D12_SHADER_BYTECODE &pixelShaderBytecode);
	bool						CreateFenceAndRootSignature();
	bool						SetAdapter(ComPtr < IDXGIAdapter1>, ComPtr < IDXGIFactory4> dxgiFactory);
	bool						setCommandqueue();
};
