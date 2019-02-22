#include "render.h"

bool Render::InitD3D(int Width, int Height, HWND &hwnd, bool FullScreen, bool Running)
{
	HRESULT hr;
	this->width = Width;
	this->height = Height;

	// -- 建立工厂 -- //

	IDXGIFactory4* dxgiFactory;
	ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory)));

	// -- 建立适配器 -- //
	IDXGIAdapter1* adapter = nullptr; // 显卡适配器

	//寻找显卡
	IF_FALSE_RETURN_FALSE(SetAdapter(adapter, dxgiFactory));



	// -- 设置Commandqueue -- //
	IF_FALSE_RETURN_FALSE(setCommandqueue());

	//建立交换链
	CreateSwapChain(hwnd, FullScreen, dxgiFactory);

	CreateRtvDescriptor();

	// create the command list with the first allocator
	ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator[frameIndex], NULL, IID_PPV_ARGS(&commandList)));

	IF_FALSE_RETURN_FALSE(CreateFenceAndRootSignature());
	D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
	compileVertexShader(vertexShaderBytecode);
	D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
	compilePixelShader(pixelShaderBytecode);
	CreatePsoAndInputLayout(vertexShaderBytecode, pixelShaderBytecode);

	//设置视口大小
	SetViewport();

	//设置裁剪空间大小
	SetScissorRect();
	//设置cameraprojmat
	XMMATRIX tmpMat = XMMatrixPerspectiveFovLH(45.0f*(3.14f / 180.0f), (float)Width / (float)Height, 0.1f, 1000.0f);
	XMStoreFloat4x4(&cameraProjMat, tmpMat);

	// 设置初始的camera状态
	cameraPosition = XMFLOAT4(0.0f, 2.0f, -4.0f, 0.0f);
	cameraTarget = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	cameraUp = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	// 建立cameraview
	XMVECTOR cPos = XMLoadFloat4(&cameraPosition);
	XMVECTOR cTarg = XMLoadFloat4(&cameraTarget);
	XMVECTOR cUp = XMLoadFloat4(&cameraUp);
	tmpMat = XMMatrixLookAtLH(cPos, cTarg, cUp);
	XMStoreFloat4x4(&cameraViewMat, tmpMat);
	return true;
}

void Render::LoadMesh(OCMesh &one)
{
	//将mesh增加到队尾
	renderMesh.push_back(one);
	//得到位置
	XMVECTOR posVec = XMLoadFloat4(&one.MTransform.Position);
	//得到位置矩阵
	auto tmpMat = XMMatrixTranslationFromVector(posVec);
	//设置mesh的rot矩阵
	XMStoreFloat4x4(&one.MTransform.RotMat, XMMatrixIdentity());
	//设置mesh的WorldMat
	XMStoreFloat4x4(&one.MTransform.WorldMat, tmpMat);
	one.RegistereForRender(device,commandList);
}

void Render::LoadMeshEnd()
{
	SetdepthStencil();
	CreateConstantBuffer();

}

void Render::Update()
{
	for (int i=0;i!= renderMesh.size();i++)
	{
		// 建立旋转矩阵
		XMMATRIX rotXMat = XMMatrixRotationX(0.0001f);
		XMMATRIX rotYMat = XMMatrixRotationY(0.0002f);
		XMMATRIX rotZMat = XMMatrixRotationZ(0.0003f);
		XMMATRIX rotMat = XMLoadFloat4x4(&renderMesh[i].MTransform.RotMat) * rotXMat * rotYMat * rotZMat;
		XMStoreFloat4x4(&renderMesh[i].MTransform.RotMat, rotMat);

		XMMATRIX translationMat = XMMatrixTranslationFromVector(XMLoadFloat4(&renderMesh[i].MTransform.Position));

		XMMATRIX worldMat = rotMat * translationMat;
		XMStoreFloat4x4(&renderMesh[i].MTransform.WorldMat, worldMat);
		// 更新每一个constant buffer
		//建立mvp矩阵
		XMMATRIX viewMat = XMLoadFloat4x4(&cameraViewMat); 
		XMMATRIX projMat = XMLoadFloat4x4(&cameraProjMat);
		XMMATRIX wvpMat = XMLoadFloat4x4(&renderMesh[i].MTransform.WorldMat) * viewMat * projMat; 
		XMMATRIX transposed = XMMatrixTranspose(wvpMat); // must transpose wvp matrix for the gpu
		XMStoreFloat4x4(&cbPerObject.wvpMat, transposed);
	}

}


void Render::SetdepthStencil()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	auto hr = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsDescriptorHeap));
	if (FAILED(hr))
	{
		Running = false;
	}
	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&depthStencilBuffer)
	);
	hr = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsDescriptorHeap));
	if (FAILED(hr))
	{
		Running = false;
	}
	dsDescriptorHeap->SetName(L"Depth/Stencil Resource Heap");

	device->CreateDepthStencilView(depthStencilBuffer, &depthStencilDesc, dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}


void Render::UpdatePipeline()
{
	HRESULT hr;
	//等待GPU完成
	WaitForPreviousFrame();

	//将分配器重置
	CHECK_HR_RUN(commandAllocator[frameIndex]->Reset());
	CHECK_HR_RUN(commandList->Reset(commandAllocator[frameIndex], pipelineStateObject));

	//将渲染目标的状态从 present state 变为 render target state 
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	//获得rtv句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);

	//得到深度测试句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	//设置
	commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
	//清空
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// 清空深度测试
	commandList->ClearDepthStencilView(dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 设置root signature
	commandList->SetGraphicsRootSignature(rootSignature); 

	// draw triangle
	commandList->RSSetViewports(1, &viewport); 
	commandList->RSSetScissorRects(1, &scissorRect); 
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 


	for (int i = 0; i != renderMesh.size(); i++)
	{

		commandList->IASetVertexBuffers(0, 1, &renderMesh[i].vertexBufferView); // set the vertex buffer (using the vertex buffer view)
		commandList->IASetIndexBuffer(&renderMesh[i].indexBufferView);
		commandList->SetGraphicsRootConstantBufferView(0, constantBufferUploadHeaps[frameIndex]->GetGPUVirtualAddress() +i* ConstantBufferPerObjectAlignedSize);
		commandList->DrawIndexedInstanced(renderMesh[i].iList.size(), 1, 0, 0, 0);
	}

	//调整状态
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	auto thr = commandList->Close();
	//CHECK_HR_RUN(commandList->Close());
	
}

void Render::run()
{
	HRESULT hr;
	//更新pso
	UpdatePipeline(); 

	// create an array of command lists (only one command list here)
	ID3D12CommandList* ppCommandLists[] = { commandList };

	// execute the array of command lists
	commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// this command goes in at the end of our command queue. we will know when our command queue
	// has finished because the fence value will be set to "fenceValue" from the GPU since the command
	// queue is being executed on the GPU
	hr = commandQueue->Signal(fence[frameIndex], fenceValue[frameIndex]);
	if (FAILED(hr))
	{
		Running = false;
	}

	// present the current backbuffer
	hr = swapChain->Present(0, 0);
	if (FAILED(hr))
	{
		Running = false;
	}
}

void Render::Cleanup()
{
	// wait for the gpu to finish all frames
	for (int i = 0; i < frameBufferCount; ++i)
	{
		frameIndex = i;
		WaitForPreviousFrame();
	}

	// get swapchain out of full screen before exiting
	BOOL fs = false;
	if (swapChain->GetFullscreenState(&fs, NULL))
		swapChain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(device);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(commandQueue);
	SAFE_RELEASE(rtvDescriptorHeap);
	SAFE_RELEASE(commandList);

	for (int i = 0; i < frameBufferCount; ++i)
	{
		SAFE_RELEASE(renderTargets[i]);
		SAFE_RELEASE(commandAllocator[i]);
		SAFE_RELEASE(fence[i]);
	};

	SAFE_RELEASE(pipelineStateObject);
	SAFE_RELEASE(rootSignature);

	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(dsDescriptorHeap);
	for (int i = 0; i < frameBufferCount; ++i)
	{
		SAFE_RELEASE(constantBufferUploadHeaps[i]);
	};
}

void Render::WaitForPreviousFrame()
{
	HRESULT hr;

	
	frameIndex = swapChain->GetCurrentBackBufferIndex();

	if (fence[frameIndex]->GetCompletedValue() < fenceValue[frameIndex])
	{
		//将围栏值设为当前值，并设置事件
		CHECK_HR_RUN( fence[frameIndex]->SetEventOnCompletion(fenceValue[frameIndex], fenceEvent));
		//直到事件触发
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	fenceValue[frameIndex]++;
}

Render::Render()
{
}

Render::~Render()
{
}

bool Render::SetAdapter(IDXGIAdapter1* adapter, IDXGIFactory4* dxgiFactory)
{
	int adapterIndex = 0;

	bool adapterFound = false;

	//遍历寻找d3d12的适配器
	while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			//我们不要软件的
			continue;
		}

		// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
		auto hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			adapterFound = true;
			break;
		}

		adapterIndex++;
	}

	if (!adapterFound)
	{
		return false;
	}

	// -- 设置device -- //
	IF_FALSE_RETURN_FALSE(SetDevice(adapter));
	return true;
}

bool Render::SetDevice(IDXGIAdapter1* adapter)
{
	if (FAILED(D3D12CreateDevice(
		adapter,
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&device)
	)))
	{
		return false;
	}
	return true;
}

bool Render::setCommandqueue()
{
	D3D12_COMMAND_QUEUE_DESC cqDesc = {};
	cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // 立即执行

	//只有一个queue
	if (FAILED(device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&commandQueue))))
	{
		return false;
	}

	//有多少个commandlist就有多少个CommandAllocator
	for (int i = 0; i < frameBufferCount; i++)
	{
		if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator[i]))))
		{
			return false;
		}
	}

	return true;
}

void Render::CreateSwapChain(HWND &hwnd, bool FullScreen, IDXGIFactory4* dxgiFactory)
{
	// 设置显示模式
	DXGI_MODE_DESC backBufferDesc = {};
	backBufferDesc.Width = width;
	backBufferDesc.Height = height;
	backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//设置采样模式
	sampleDesc.Count = 1;

	//交换链描述
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = frameBufferCount; //buffer数量
	swapChainDesc.BufferDesc = backBufferDesc; // 显示模式赋值
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // TARGET_OUTPUT
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
	swapChainDesc.OutputWindow = hwnd; // handle to our window
	swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
	swapChainDesc.Windowed = !FullScreen; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

	IDXGISwapChain* tempSwapChain;

	dxgiFactory->CreateSwapChain(
		commandQueue, // the queue will be flushed once the swap chain is created
		&swapChainDesc, // give it the swap chain description we created above
		&tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
	);

	swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
	frameIndex = swapChain->GetCurrentBackBufferIndex();
}

void Render::CreateRtvDescriptor()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = frameBufferCount; // 堆的数量
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));

	rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//得到栈顶的handle
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	// Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
	for (int i = 0; i < frameBufferCount; i++)
	{
		// first we get the n'th buffer in the swap chain and store it in the n'th
		// position of our ID3D12Resource array
		swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));

		// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
		device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);

		// we increment the rtv handle by the rtv descriptor size we got above
		rtvHandle.Offset(1, rtvDescriptorSize);
	}
}

void Render::CreateConstantBuffer()
{

	for (int i = 0; i < frameBufferCount; ++i)
	{
		// 建立upload
		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64), //大小，必须是64kb倍数
			D3D12_RESOURCE_STATE_GENERIC_READ, // 读取
			nullptr,
			IID_PPV_ARGS(&constantBufferUploadHeaps[i]));
		constantBufferUploadHeaps[i]->SetName(L"Constant Buffer Upload Resource Heap");

		ZeroMemory(&cbPerObject, sizeof(cbPerObject));
		//我们不需要读取，只需要写入
		CD3DX12_RANGE readRange(0, 0);

		//使其获取到内存中

		for (int j = 0; j != renderMesh.size(); j++)
		{
			constantBufferUploadHeaps[i]->Map(0, &readRange, reinterpret_cast<void**>(&cbvGPUAddress[i]));
			memcpy(cbvGPUAddress[i] + ConstantBufferPerObjectAlignedSize * j, &cbPerObject, sizeof(cbPerObject)); 

		}
	}

	commandList->Close();
	ID3D12CommandList* ppCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	fenceValue[frameIndex]++;
	CHECK_HR_RUN(commandQueue->Signal(fence[frameIndex], fenceValue[frameIndex]));


}


bool Render::CreateFenceAndRootSignature()
{
	HRESULT hr;
	for (int i = 0; i < frameBufferCount; i++)
	{
		hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence[i]));
		if (FAILED(hr))
		{
			return false;
		}
		fenceValue[i] = 0; // set the initial fence value to 0
	}

	// create a handle to a fence event
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (fenceEvent == nullptr)
	{
		return false;
	}

	// create root signature

	// create a root descriptor, which explains where to find the data for this root parameter
	D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
	rootCBVDescriptor.RegisterSpace = 0;
	rootCBVDescriptor.ShaderRegister = 0;

	// create a root parameter and fill it out
	D3D12_ROOT_PARAMETER  rootParameters[1]; // only one parameter right now
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // this is a constant buffer view root descriptor
	rootParameters[0].Descriptor = rootCBVDescriptor; // this is the root descriptor for this root parameter
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // our pixel shader will be the only shader accessing this parameter for now

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(_countof(rootParameters), // we have 1 root parameter
		rootParameters, // a pointer to the beginning of our root parameters array
		0,
		nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // we can deny shader stages here for better performance
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS);

	ID3DBlob* signature;
	hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Render::compileVertexShader(D3D12_SHADER_BYTECODE&vertexShaderBytecode)
{
	ID3DBlob* errorBuff;
	ID3DBlob* vertexShader;

	auto hr = D3DCompileFromFile(L"VertexShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShader,
		&errorBuff);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBuff->GetBufferPointer());
		return false;
	}
	vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
	vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();
	return true;
}

bool Render::compilePixelShader(D3D12_SHADER_BYTECODE &pixelShaderBytecode)
{
	ID3DBlob* errorBuff;

	ID3DBlob* pixelShader;
	auto hr = D3DCompileFromFile(L"PixelShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShader,
		&errorBuff);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBuff->GetBufferPointer());
		return false;
	}

	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();
	return true;
}

bool Render::CreatePsoAndInputLayout(D3D12_SHADER_BYTECODE &vertexShaderBytecode, D3D12_SHADER_BYTECODE &pixelShaderBytecode)
{
	//设置IL，只有两个参数
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{"COLOR",0,     DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
	};

	//设置IL的desc
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	inputLayoutDesc.pInputElementDescs = inputLayout;

	//设置psoDesc
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = inputLayoutDesc; // IL描述
	psoDesc.pRootSignature = rootSignature; // 设置root signature
	psoDesc.VS = vertexShaderBytecode; //编译过的Vs
	psoDesc.PS = pixelShaderBytecode; //
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // draw的类型
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //  render target类型
	psoDesc.SampleDesc = sampleDesc; // 采样类型
	psoDesc.SampleMask = 0xffffffff; //
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // 默认
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // 默认
	psoDesc.NumRenderTargets = 1; //只绑定以个
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); //默认的深度测试

	// 建立pso
	auto hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineStateObject));
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void Render::SetScissorRect()
{
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = width;
	scissorRect.bottom = height;
}

void Render::SetViewport()
{
	//默认的viewport
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}