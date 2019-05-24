#include "Engine.h"
#include"OString.h"
#include"RotationMatrix.h"
#include "EngineBase.h"
const int gNumFrameResources = 3;

Engine::Engine(HINSTANCE hInstance)
	: EngineBase(hInstance)
{
	mSceneBounds.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mSceneBounds.Radius = sqrtf(10.0f*10.0f + 15.0f*15.0f);
	mCamera.ProjectMode = cameraType;
}



Engine::~Engine()
{
	if (md3dDevice != nullptr)
		FlushCommandQueue();
}

void Engine::RunWithQTInOne()
{
	mTimer.Tick();
	
	if (!mEnginePaused)
	{
		CalculateFrameStats();
		Update(mTimer);
		Draw(mTimer);
		//_CrtDumpMemoryLeaks();//注意必须放在程序的结尾处
	}
}

bool Engine::Initialize()
{
	//初始化
	if (!EngineBase::Initialize())
		return false;
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
	if (CameraDefault)
	{
		mCamera.SetPosition(0.0f, 2.0f, -15.0f);
		mCamera.Pitch(30);
	}

		mShadowMap = std::make_unique<ShadowMap>(
			md3dDevice.Get(), 2048, 2048);
	LoadTextures();
	BuildRootSignature();
	BuildDescriptorHeaps();
	BuildShadersAndInputLayout();
	BuildShapeGeometry();
	BuildSkullGeometry();
	BuildGrip();
	BuildMaterials();
	BuildRenderItems();
	BuildFrameResources();
	BuildPSOs();

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();

	return true;
}

bool Engine::Initialize(HWND Bwindows)
{

	if (!EngineBase::Initialize(Bwindows))
		return false;
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	mCamera.SetPosition(0.0f, 2.0f, -15.0f);

	mShadowMap = std::make_unique<ShadowMap>(
		md3dDevice.Get(), 2048, 2048);

	LoadTextures();
	BuildRootSignature();
	BuildDescriptorHeaps();
	BuildShadersAndInputLayout();
	BuildShapeGeometry();
	BuildSkullGeometry();
	BuildGrip();
	BuildMaterials();
	BuildRenderItems();
	BuildFrameResources();
	BuildPSOs();

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();

	return true;
}

void Engine::OnResize()
{
	EngineBase::OnResize();

	mCamera.SetLens(0.25f*PI, AspectRatio(), 1.0f, 1000.0f);
}

void Engine::BuildGrip()
{
	//建立基本的网格信息
	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "Grip";

	vector<Vertex>Points;
	std::vector<std::int32_t>Indexs;

	int rowNum = 400;
	int colNum = 400;

	for (int i = -100; i != 100; i++)
	{
		Vertex ppp;
		ppp.Pos = Vector(-100, i, 0);
		Points.push_back(ppp);
		ppp.Pos = Vector(100, i, 0);
		Points.push_back(ppp);
		Indexs.push_back((i+100)*2);
		Indexs.push_back((i + 100) *2+1);
	}

	for (int i = -100; i != 100; i++)
	{
		Vertex ppp;
		ppp.Pos = Vector(i, 100, 0);
		Points.push_back(ppp);
		ppp.Pos = Vector(i, -100, 0);
		Points.push_back(ppp);
		Indexs.push_back((i + 100) * 2+ rowNum);
		Indexs.push_back((i + 100) * 2 + 1+ rowNum);
	}

	

	const UINT vbByteSize = (UINT)Points.size() * sizeof(Vertex);

	const UINT ibByteSize = (UINT)Indexs.size() * sizeof(std::int32_t);

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), Points.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), Indexs.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), Points.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), Indexs.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	geo->IndexBufferByteSize = ibByteSize;


	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)Indexs.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	//submesh.Bounds = bounds;

	geo->DrawArgs["Grip"] = submesh;

	mGeometries[geo->Name] = std::move(geo);
}

void Engine::GetGeometry(vector<Vertex> Plist,string Name)
{
	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = Name;
	//auto Points = Plist;
	auto Points = Plist;
	Points.push_back(Vertex());
	Points[0].Pos = Vector(-100,-100, 0);
	Points[1].Pos = Vector(100, 100, 0);
	std::vector<std::int32_t>Indexs;
	for (int i = 0; i != Points.size();i++)
	{
		if (Points.size() == 1)
		{
			Indexs.push_back(i);
			break;
		}
		Indexs.push_back(i);

		//Indexs.push_back(i+1);
	}

	const UINT vbByteSize = (UINT)Points.size() * sizeof(Vertex);

	const UINT ibByteSize = (UINT)Indexs.size() * sizeof(std::int32_t);

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), Points.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), Indexs.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), Points.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), Indexs.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	geo->IndexBufferByteSize = ibByteSize;


	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)Indexs.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	//submesh.Bounds = bounds;

	geo->DrawArgs[Name] = submesh;

	mGeometries[geo->Name] = std::move(geo);
}

void Engine::Update(const GameTimer& gt)
{
	OnKeyboardInput(gt);

	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();//得到当前帧的渲染资源

	if (mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
	mLightRotationAngle += 0.1f*gt.DeltaTime();

	Matrix R = RotationMatrix(Rotator(mLightRotationAngle*180/PI,0,0 ));
	for (int i = 0; i < 3; ++i)
	{
		Vector lightDir = mBaseLightDirections[i];
		lightDir = R.TransformPosition(lightDir)  ;
		mRotatedLightDirections[i]= lightDir;
	}

	AnimateMaterials(gt);
	UpdateObjectCBs(gt);
	UpdateMaterialBuffer(gt);
	UpdateShadowTransform(gt);
	UpdateMainPassCB(gt);
	UpdateShadowPassCB(gt);
}

void Engine::Draw(const GameTimer& gt)
{
	//重置分配器
	auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;
	ThrowIfFailed(cmdListAlloc->Reset());
	ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque"].Get()));
	//设置RootSignature
	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvDescriptorHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	//绑定material
	auto matBuffer = mCurrFrameResource->MaterialBuffer->Resource();
	mCommandList->SetGraphicsRootShaderResourceView(2, matBuffer->GetGPUVirtualAddress());

	//设置shadowbuffer
	
		mCommandList->SetGraphicsRootDescriptorTable(3, mNullSrv);
		//设置texture
		mCommandList->SetGraphicsRootDescriptorTable(4, mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		DrawSceneToShadowMap();
	
	

	


	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	auto passCB = mCurrFrameResource->PassCB->Resource();
	mCommandList->SetGraphicsRootConstantBufferView(1, passCB->GetGPUVirtualAddress());


	//设置天空
	CD3DX12_GPU_DESCRIPTOR_HANDLE skyTexDescriptor(mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	skyTexDescriptor.Offset(mSkyTexHeapIndex, mCbvSrvUavDescriptorSize);
	mCommandList->SetGraphicsRootDescriptorTable(3, skyTexDescriptor);

	mCommandList->SetPipelineState(mPSOs["opaque"].Get());
	DrawRenderItems(mCommandList.Get(), mRitemLayer[(int)RenderLayer::Opaque]);
	if (IsDebugShadow)
	{
		mCommandList->SetPipelineState(mPSOs["debug"].Get());
		DrawRenderItems(mCommandList.Get(), mRitemLayer[(int)RenderLayer::Debug]);
	}

	mCommandList->SetPipelineState(mPSOs["sky"].Get());
	DrawRenderItems(mCommandList.Get(), mRitemLayer[(int)RenderLayer::Sky]);


	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	//结束
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	//交换
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
	//设置同步
	mCurrFrameResource->Fence = ++mCurrentFence;
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);
	//mCommandQueue->Flush();
}

void Engine::OnMouseDown(WPARAM btnState, int x, int y)
{
	//ReleaseCapture();
	if (DawLineGame)
	{
		Matrix view = mCamera.GetView().Inverse();
		Matrix proj = mCamera.GetProj().Inverse();
		mDrawLineControl.TransformScreenPosition(x, y, view*proj);
		
		if (mDrawLineControl.isHaveRenderItem)
		{
			
			/*
			mGeometries.erase(mDrawLineControl.LineName);
			auto LineMesh = GetGeometry(mDrawLineControl.Plist, mDrawLineControl.LineName);
			mGeometries[LineMesh->Name] = std::move(LineMesh);

			UpdateRenderItems(mDrawLineControl.RenderLinePlane, mGeometries[mDrawLineControl.LineName].get(), D3D_PRIMITIVE_TOPOLOGY_LINELIST, mDrawLineControl.LineName);*/
			

		}
		else
		{
			GetGeometry(mDrawLineControl.Plist, mDrawLineControl.LineName);
			
			BUildOneRenderItem(Matrix::Identity
				, "tile0", mDrawLineControl.LineName, D3D_PRIMITIVE_TOPOLOGY_LINELIST
				, RenderLayer::Opaque, mDrawLineControl.LineName);
			//mAllRitems[mAllRitems.size() - 1]->NumFramesDirty = 0;
			mDrawLineControl.isHaveRenderItem = true;
			BuildFrameResources();
		/*	for (int i = 0; i != mFrameResources.size(); i++)
			{
				mFrameResources[i]->ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), (UINT)mAllRitems.size(), true);
			}
			isInAddMesh = true;*/
		
		}
	}
}

void Engine::OnMouseUp(WPARAM btnState, int x, int y)
{

}

void Engine::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (GameMoudle)
	{
		RECT  ss;
		GetWindowRect(mhMainWnd, &ss);
		POINT cp;

		GetCursorPos(&cp);
		x = cp.x;
		y = cp.y;
		float PointX = (ss.right - ss.left) / 2 + ss.left;
		float Pointy = (ss.bottom - ss.top) / 2 + ss.top;


		float dx = XMConvertToRadians(static_cast<float>(PointX - x));
		float dy = XMConvertToRadians(static_cast<float>(Pointy - y));



		mCamera.Pitch(dy);
		mCamera.RotateY(dx);
		SetCursorPos(PointX, Pointy);
	}


}

void Engine::OnKeyboardInput(const GameTimer& gt)
{
	const float dt = gt.DeltaTime();

	if (GetAsyncKeyState('W') & 0x8000)
		mCamera.Walk(SPEED_ON_BOARD*dt);

	if (GetAsyncKeyState('S') & 0x8000)
		mCamera.Walk(-SPEED_ON_BOARD *dt);

	if (GetAsyncKeyState('A') & 0x8000)
		mCamera.Strafe(-SPEED_ON_BOARD *dt);

	if (GetAsyncKeyState('D') & 0x8000)
		mCamera.Strafe(SPEED_ON_BOARD*dt);

	mCamera.UpdateViewMatrix();
}

void Engine::AnimateMaterials(const GameTimer& gt)
{
}

void Engine::UpdateObjectCBs(const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	if (isInAddMesh == true)
	{
		for (auto& e : mAllRitems)
		{
			e->NumFramesDirty = 3;
		}
		isInAddMesh = false;

	}
	for (auto& e : mAllRitems)
	{
		//只在脏的时候修改
		if (e->NumFramesDirty > 0)
		{
			Matrix world = e->World;
			Matrix texTransform = e->TexTransform;

			ObjectConstants objConstants;
			objConstants.World = world.GetTransposed();
			objConstants.TexTransform = texTransform.GetTransposed();
			objConstants.MaterialIndex = e->Mat->MatCBIndex;

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);
			e->NumFramesDirty--;
		}
	}
	
}

void Engine::UpdateMaterialBuffer(const GameTimer& gt)
{
	auto currMaterialBuffer = mCurrFrameResource->MaterialBuffer.get();
	for (auto& e : mMaterials)
	{
		// Only update the cbuffer data if the constants have changed.  If the cbuffer
		// data changes, it needs to be updated for each FrameResource.
		Material* mat = e.second.get();
		if (mat->NumFramesDirty > 0)
		{
			Matrix matTransform = mat->MatTransform;

			MaterialData matData;
			matData.DiffuseAlbedo = mat->DiffuseAlbedo;
			matData.FresnelR0 = mat->FresnelR0;
			matData.Roughness = mat->Roughness;
			matData.MatTransform=matTransform.GetTransposed();
			matData.DiffuseMapIndex = mat->DiffuseSrvHeapIndex;
			matData.NormalMapIndex = mat->NormalSrvHeapIndex;

			currMaterialBuffer->CopyData(mat->MatCBIndex, matData);

			// Next FrameResource need to be updated too.
			mat->NumFramesDirty--;
		}
	}
}

void Engine::UpdateMainPassCB(const GameTimer& gt)
{
	Matrix view = mCamera.GetView();
	Matrix proj = mCamera.GetProj();

	Matrix viewProj = view * proj;
	Matrix invView = view.Inverse();
	Matrix invProj = proj.Inverse();
	Matrix invViewProj = viewProj.Inverse();
	Matrix shadowTransform = mShadowTransform;
	mMainPassCB.View = view.GetTransposed();
	mMainPassCB.InvView = (invView).GetTransposed();
	mMainPassCB.Proj = (proj).GetTransposed();
	mMainPassCB.InvProj = (invProj).GetTransposed();
	mMainPassCB.ViewProj = (viewProj).GetTransposed();
	mMainPassCB.InvViewProj = (invViewProj).GetTransposed();
	mMainPassCB.ShadowTransform= shadowTransform.GetTransposed();
	mMainPassCB.EyePosW = mCamera.GetPosition();
	mMainPassCB.RenderTargetSize = Vector2D((float)mClientWidth, (float)mClientHeight);
	mMainPassCB.InvRenderTargetSize = Vector2D(1.0f / mClientWidth, 1.0f / mClientHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = gt.TotalTime();
	mMainPassCB.DeltaTime = gt.DeltaTime();
	mMainPassCB.AmbientLight = Vector4{ 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.Lights[0].Direction = mRotatedLightDirections[0];
	mMainPassCB.Lights[0].Strength = { 0.9f, 0.8f, 0.7f };
	mMainPassCB.Lights[1].Direction = mRotatedLightDirections[1];
	mMainPassCB.Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
	mMainPassCB.Lights[2].Direction = mRotatedLightDirections[2];
	mMainPassCB.Lights[2].Strength = { 0.2f, 0.2f, 0.2f };
	mMainPassCB.Reflection = Reflection;
	mMainPassCB.feature_shadow = feature_shadow;
	mMainPassCB.SkyCube = SkyCube;
	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void Engine::LoadTextures()
{
	std::vector<std::string> texNames =
	{
		"bricksDiffuseMap",
		"bricksNormalMap",
		"tileDiffuseMap",
		"tileNormalMap",
		"defaultDiffuseMap",
		"defaultNormalMap",
		"skyCubeMap"
	};

	std::vector<std::wstring> texFilenames =
	{
		  L"../Textures/bricks2.dds",
		  L"../Textures/bricks2_nmap.dds",
		  L"../Textures/tile.dds",
		  L"../Textures/tile_nmap.dds",
		  L"../Textures/white1x1.dds",
		  L"../Textures/default_nmap.dds",
		  L"../Textures/desertcube1024.dds"
	};

	for (int i = 0; i < (int)texNames.size(); ++i)
	{
		auto texMap = std::make_unique<Texture>();
		texMap->Name = texNames[i];
		texMap->Filename = texFilenames[i];
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice.Get(),
			mCommandList.Get(), texMap->Filename.c_str(),
			texMap->Resource, texMap->UploadHeap));

		mTextures[texMap->Name] = std::move(texMap);
	}
}

void Engine::BuildRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE texTable0;
	texTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0);

	CD3DX12_DESCRIPTOR_RANGE texTable1;
	texTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 10, 2, 0);

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[5];

	// Perfomance TIP: Order from most frequent to least frequent.
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);
	slotRootParameter[2].InitAsShaderResourceView(0, 1);
	slotRootParameter[3].InitAsDescriptorTable(1, &texTable0, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[4].InitAsDescriptorTable(1, &texTable1, D3D12_SHADER_VISIBILITY_PIXEL);


	auto staticSamplers = GetStaticSamplers();

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(5, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf())));
}

void Engine::BuildDescriptorHeaps()
{
	//
		// Create the SRV heap.
		//
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 14;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvDescriptorHeap)));

	//
	// Fill out the heap with actual descriptors.
	//
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	std::vector<ComPtr<ID3D12Resource>> tex2DList =
	{
		mTextures["bricksDiffuseMap"]->Resource,
		mTextures["bricksNormalMap"]->Resource,
		mTextures["tileDiffuseMap"]->Resource,
		mTextures["tileNormalMap"]->Resource,
		mTextures["defaultDiffuseMap"]->Resource,
		mTextures["defaultNormalMap"]->Resource
	};

	auto skyCubeMap = mTextures["skyCubeMap"]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	for (UINT i = 0; i < (UINT)tex2DList.size(); ++i)
	{
		srvDesc.Format = tex2DList[i]->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = tex2DList[i]->GetDesc().MipLevels;
		md3dDevice->CreateShaderResourceView(tex2DList[i].Get(), &srvDesc, hDescriptor);

		// next descriptor
		hDescriptor.Offset(1, mCbvSrvUavDescriptorSize);
	}

	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = skyCubeMap->GetDesc().MipLevels;
	srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	srvDesc.Format = skyCubeMap->GetDesc().Format;
	md3dDevice->CreateShaderResourceView(skyCubeMap.Get(), &srvDesc, hDescriptor);

	mSkyTexHeapIndex = (UINT)tex2DList.size();
	mShadowMapHeapIndex = mSkyTexHeapIndex + 1;

	mNullCubeSrvIndex = mShadowMapHeapIndex + 1;
	mNullTexSrvIndex = mNullCubeSrvIndex + 1;

	auto srvCpuStart = mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto srvGpuStart = mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	auto dsvCpuStart = mDsvHeap->GetCPUDescriptorHandleForHeapStart();


	auto nullSrv = CD3DX12_CPU_DESCRIPTOR_HANDLE(srvCpuStart, mNullCubeSrvIndex, mCbvSrvUavDescriptorSize);
	mNullSrv = CD3DX12_GPU_DESCRIPTOR_HANDLE(srvGpuStart, mNullCubeSrvIndex, mCbvSrvUavDescriptorSize);

	md3dDevice->CreateShaderResourceView(nullptr, &srvDesc, nullSrv);
	nullSrv.Offset(1, mCbvSrvUavDescriptorSize);

	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	md3dDevice->CreateShaderResourceView(nullptr, &srvDesc, nullSrv);

	mShadowMap->BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE(srvCpuStart, mShadowMapHeapIndex, mCbvSrvUavDescriptorSize),
		CD3DX12_GPU_DESCRIPTOR_HANDLE(srvGpuStart, mShadowMapHeapIndex, mCbvSrvUavDescriptorSize),
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvCpuStart, 1, mDsvDescriptorSize));
}

void Engine::BuildShadersAndInputLayout()
{
	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"ALPHA_TEST", "1",
		NULL, NULL
	};

	mShaders["standardVS"] = d3dUtil::CompileShader(L"../Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"../Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");

	mShaders["shadowVS"] = d3dUtil::CompileShader(L"../Shaders\\Shadows.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["shadowOpaquePS"] = d3dUtil::CompileShader(L"../Shaders\\Shadows.hlsl", nullptr, "PS", "ps_5_1");
	mShaders["shadowAlphaTestedPS"] = d3dUtil::CompileShader(L"../Shaders\\Shadows.hlsl", alphaTestDefines, "PS", "ps_5_1");

	mShaders["debugVS"] = d3dUtil::CompileShader(L"../Shaders\\ShadowDebug.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["debugPS"] = d3dUtil::CompileShader(L"../Shaders\\ShadowDebug.hlsl", nullptr, "PS", "ps_5_1");

	mShaders["skyVS"] = d3dUtil::CompileShader(L"../Shaders\\Sky.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["skyPS"] = d3dUtil::CompileShader(L"../Shaders\\Sky.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}

void Engine::BuildShapeGeometry()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 3);
	GeometryGenerator::MeshData grid = geoGen.CreateGrid(20.0f, 30.0f, 60, 40);
	GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);
	GeometryGenerator::MeshData quad = geoGen.CreateQuad(0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	//
	// We are concatenating all the geometry into one big vertex/index buffer.  So
	// define the regions in the buffer each submesh covers.
	//

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	UINT boxVertexOffset = 0;
	UINT gridVertexOffset = (UINT)box.Vertices.size();
	UINT sphereVertexOffset = gridVertexOffset + (UINT)grid.Vertices.size();
	UINT cylinderVertexOffset = sphereVertexOffset + (UINT)sphere.Vertices.size();
	UINT quadVertexOffset = cylinderVertexOffset + (UINT)cylinder.Vertices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	UINT boxIndexOffset = 0;
	UINT gridIndexOffset = (UINT)box.Indices32.size();
	UINT sphereIndexOffset = gridIndexOffset + (UINT)grid.Indices32.size();
	UINT cylinderIndexOffset = sphereIndexOffset + (UINT)sphere.Indices32.size();
	UINT quadIndexOffset = cylinderIndexOffset + (UINT)cylinder.Indices32.size();

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry gridSubmesh;
	gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
	gridSubmesh.StartIndexLocation = gridIndexOffset;
	gridSubmesh.BaseVertexLocation = gridVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	SubmeshGeometry cylinderSubmesh;
	cylinderSubmesh.IndexCount = (UINT)cylinder.Indices32.size();
	cylinderSubmesh.StartIndexLocation = cylinderIndexOffset;
	cylinderSubmesh.BaseVertexLocation = cylinderVertexOffset;

	SubmeshGeometry quadSubmesh;
	quadSubmesh.IndexCount = (UINT)quad.Indices32.size();
	quadSubmesh.StartIndexLocation = quadIndexOffset;
	quadSubmesh.BaseVertexLocation = quadVertexOffset;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	auto totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size() +
		quad.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].TexC = box.Vertices[i].TexC;
		vertices[k].TangentU = box.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].TexC = grid.Vertices[i].TexC;
		vertices[k].TangentU = grid.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		vertices[k].TexC = sphere.Vertices[i].TexC;
		vertices[k].TangentU = sphere.Vertices[i].TangentU;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		vertices[k].TexC = cylinder.Vertices[i].TexC;
		vertices[k].TangentU = cylinder.Vertices[i].TangentU;
	}

	for (int i = 0; i < quad.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = quad.Vertices[i].Position;
		vertices[k].Normal = quad.Vertices[i].Normal;
		vertices[k].TexC = quad.Vertices[i].TexC;
		vertices[k].TangentU = quad.Vertices[i].TangentU;
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
	indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
	indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));
	indices.insert(indices.end(), std::begin(quad.GetIndices16()), std::end(quad.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "shapeGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["box"] = boxSubmesh;
	geo->DrawArgs["grid"] = gridSubmesh;
	geo->DrawArgs["sphere"] = sphereSubmesh;
	geo->DrawArgs["cylinder"] = cylinderSubmesh;
	geo->DrawArgs["quad"] = quadSubmesh;

	mGeometries[geo->Name] = std::move(geo);
}

void Engine::BuildSkullGeometry()
{
	std::ifstream fin("../Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"../Models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	Vector vMinf3(+Math::Infinity, +Math::Infinity, +Math::Infinity);
	Vector vMaxf3(-Math::Infinity, -Math::Infinity, -Math::Infinity);

	Vector vMin = vMinf3;
	Vector vMax = vMaxf3;

	std::vector<Vertex> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.X >> vertices[i].Pos.Y >> vertices[i].Pos.Z;
		fin >> vertices[i].Normal.X>> vertices[i].Normal.Y >> vertices[i].Normal.Z;

		vertices[i].TexC = { 0.0f, 0.0f };

		Vector P = vertices[i].Pos;

		Vector N = vertices[i].Normal;

		Vector up = Vector(0.0f, 1.0f, 0.0f);
		if (fabsf(((N| up))) < 1.0f - 0.001f)
		{
			Vector T = ((up^ N)).GetSafeNormal();
			vertices[i].TangentU= T;
		}
		else
		{
			up = Vector(0.0f, 0.0f, 1.0f);
			Vector T = ((N^up)).GetSafeNormal();
			vertices[i].TangentU= T;
		}


		vMin = Vector::Vector3dMin(vMin, P);
		vMax = Vector::Vector3dMax(vMax, P);
	}

	BoundingBox bounds;

	bounds.Center = (0.5f*(vMin + vMax)).GetXMFLOAT3();
	bounds.Extents = (0.5f*(vMax - vMin)).GetXMFLOAT3();

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	std::vector<std::int32_t> indices(3 * tcount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);

	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::int32_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "skullGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	submesh.Bounds = bounds;

	geo->DrawArgs["skull"] = submesh;

	mGeometries[geo->Name] = std::move(geo);
}


void Engine::BuildOBJMesh(string Flie, string FliePath)
{
	std::ifstream fin(FliePath+Flie);
	string ErrorMessage = Flie + string("Not find");
	if (!fin)
	{
		//MessageBox(0, OString::multi_Byte_To_Wide_Char(ErrorMessage), 0, 0);
		return;
	}

	Vector vMinf3(+Math::Infinity, +Math::Infinity, +Math::Infinity);
	Vector vMaxf3(-Math::Infinity, -Math::Infinity, -Math::Infinity);


	BoundingBox bounds;

	string s;
	vector<Vector>Vetexs;//顶点
	vector<Vector>Normals;//顶点
	vector<Vector>Textures;//
	vector<Vertex>Points;//
	std::vector<std::int32_t>ALLIndexs;
	std::vector<std::int32_t>Indexs;
	while (getline(fin, s))
	{
		if (s.length() < 2)continue;
		if (s[0] == 'v') {
			if (s[1] == 't')
			{//vt  纹理
				istringstream in(s);
				Vector tex;
				string head;
				in >> head >> tex.X >> tex.Y >> tex.Z;
				Textures.push_back(tex);
			}
			else if (s[1] == 'n') {//vn法向量
				istringstream in(s);
				Vector normal;
				string head;
				in >> head >> normal.X >> normal.Y >> normal.Z;
				Normals.push_back(normal);
			}
			else {//v  点
				istringstream in(s);
				Vector Vetex;
				string head;

				in >> head >> Vetex.X >> Vetex.Y >> Vetex.Z;
				Vetex = Vetex / 10;
				vMinf3 = Vector::Vector3dMin(vMinf3, Vetex);
				vMaxf3= Vector::Vector3dMax(vMinf3, Vetex);
				Vetexs.push_back(Vetex);
			}
		}
		else if (s[0] == 'f') {//f 2443//2656 2442//2656 2444//2656 面
			for (int k = s.size() - 1; k >= 0; k--) {
				if (s[k] == '/')s[k] = ' ';
			}
			istringstream in(s);
			string head;
			in >> head;
			int i = 0;

			while (i < 3) {
				std::int32_t ttt;
				//Vertex tempx;
				in >> ttt;
				ttt -= 1;
				
				ALLIndexs.push_back(ttt);
				//tempx.Pos = Vetexs[ttt];
				in >> ttt;
				ttt -= 1;
				ALLIndexs.push_back(ttt);
				//tempx.TexC = { Textures[ttt].X, Textures[ttt].Y };
				in >> ttt;
				ttt -= 1;
				ALLIndexs.push_back(ttt);
				//tempx.Normal = Normals[ttt];
			
				i++;
				//Points.push_back(tempx);
			}

		}

	}

	for (int i = 0; i != ALLIndexs.size()/3; i++)
	{
		Vertex tempx;
		tempx.Pos = Vetexs[ALLIndexs[i*3]];
		tempx.TexC = { Textures[ALLIndexs[i * 3 + 1]].X, Textures[ALLIndexs[i * 3 + 1]].Y };
		//tempx.TexC = {0,0 };
		tempx.Normal = Normals[ALLIndexs[i * 3 + 2]];
		Points.push_back(tempx);
		Indexs.push_back(i);
	}

	fin.close();
	const UINT vbByteSize = (UINT)Points.size() * sizeof(Vertex);

	const UINT ibByteSize = (UINT)Indexs.size() * sizeof(std::int32_t);
	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "ssfOBJ";
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), Points.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), Indexs.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), Points.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), Indexs.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	bounds.Center = (0.5f*(vMinf3 + vMaxf3)).GetXMFLOAT3();
	bounds.Extents = (0.5f*(vMaxf3 - vMinf3)).GetXMFLOAT3();
	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)Indexs.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	submesh.Bounds = bounds;

	geo->DrawArgs["ssf"] = submesh;

	mGeometries[geo->Name] = std::move(geo);

}

void Engine::BuildPSOs()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

	//
	// PSO for opaque objects.
	//
	ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	opaquePsoDesc.pRootSignature = mRootSignature.Get();
	opaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()),
		mShaders["standardVS"]->GetBufferSize()
	};
	opaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
		mShaders["opaquePS"]->GetBufferSize()
	};
	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = mBackBufferFormat;
	opaquePsoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	opaquePsoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	opaquePsoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&mPSOs["opaque"])));

	//
	// PSO for shadow map pass.
	//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC smapPsoDesc = opaquePsoDesc;
	smapPsoDesc.RasterizerState.DepthBias = 100000;
	smapPsoDesc.RasterizerState.DepthBiasClamp = 0.0f;
	smapPsoDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;
	smapPsoDesc.pRootSignature = mRootSignature.Get();
	smapPsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["shadowVS"]->GetBufferPointer()),
		mShaders["shadowVS"]->GetBufferSize()
	};
	smapPsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["shadowOpaquePS"]->GetBufferPointer()),
		mShaders["shadowOpaquePS"]->GetBufferSize()
	};

	// Shadow map pass does not have a render target.
	smapPsoDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	smapPsoDesc.NumRenderTargets = 0;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&smapPsoDesc, IID_PPV_ARGS(&mPSOs["shadow_opaque"])));

	//
	// PSO for debug layer.
	//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC debugPsoDesc = opaquePsoDesc;
	debugPsoDesc.pRootSignature = mRootSignature.Get();
	debugPsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["debugVS"]->GetBufferPointer()),
		mShaders["debugVS"]->GetBufferSize()
	};
	debugPsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["debugPS"]->GetBufferPointer()),
		mShaders["debugPS"]->GetBufferSize()
	};
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&debugPsoDesc, IID_PPV_ARGS(&mPSOs["debug"])));

	//
	// PSO for sky.
	//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC skyPsoDesc = opaquePsoDesc;

	// The camera is inside the sky sphere, so just turn off culling.
	skyPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
	skyPsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	skyPsoDesc.pRootSignature = mRootSignature.Get();
	skyPsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["skyVS"]->GetBufferPointer()),
		mShaders["skyVS"]->GetBufferSize()
	};
	skyPsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["skyPS"]->GetBufferPointer()),
		mShaders["skyPS"]->GetBufferSize()
	};
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&skyPsoDesc, IID_PPV_ARGS(&mPSOs["sky"])));
}

void Engine::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(std::make_unique<FrameResource>(md3dDevice.Get(),
			2, (UINT)mAllRitems.size(), (UINT)mMaterials.size()));
	}
}

void Engine::BuildMaterials()
{
	auto bricks0 = std::make_unique<Material>();
	bricks0->Name = "bricks0";
	bricks0->MatCBIndex = 0;
	bricks0->DiffuseSrvHeapIndex = 0;
	bricks0->NormalSrvHeapIndex = 1;
	bricks0->DiffuseAlbedo = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	bricks0->FresnelR0 = Vector(0.1f, 0.1f, 0.1f);
	bricks0->Roughness = 0.3f;

	auto tile0 = std::make_unique<Material>();
	tile0->Name = "tile0";
	tile0->MatCBIndex = 1;
	tile0->NormalSrvHeapIndex = 2;
	tile0->DiffuseSrvHeapIndex = 1;
	tile0->DiffuseAlbedo = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	tile0->FresnelR0 = Vector(0.2f, 0.2f, 0.2f);
	tile0->Roughness = 0.1f;

	auto mirror0 = std::make_unique<Material>();
	mirror0->Name = "mirror0";
	mirror0->MatCBIndex = 2;
	mirror0->NormalSrvHeapIndex = 3;

	mirror0->DiffuseSrvHeapIndex = 2;
	mirror0->DiffuseAlbedo = Vector4(0.0f, 0.0f, 0.1f, 1.0f);
	mirror0->FresnelR0 = Vector(0.98f, 0.97f, 0.95f);
	mirror0->Roughness = 0.1f;

	auto skullMat = std::make_unique<Material>();
	skullMat->Name = "skullMat";
	skullMat->MatCBIndex = 3;
	skullMat->NormalSrvHeapIndex = 4;
	skullMat->DiffuseSrvHeapIndex = 2;
	skullMat->DiffuseAlbedo = Vector4(0.3f, 0.3f, 0.8f, 1.0f);
	skullMat->FresnelR0 = Vector(0.6f, 0.6f, 0.6f);
	skullMat->Roughness = 0.2f;

	auto sky = std::make_unique<Material>();
	sky->Name = "sky";
	sky->MatCBIndex = 4;
	sky->NormalSrvHeapIndex = 5;

	sky->DiffuseSrvHeapIndex = 3;
	sky->DiffuseAlbedo = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	sky->FresnelR0 = Vector(0.1f, 0.1f, 0.1f);
	sky->Roughness = 1.0f;

	mMaterials["bricks0"] = std::move(bricks0);
	mMaterials["tile0"] = std::move(tile0);
	mMaterials["mirror0"] = std::move(mirror0);
	mMaterials["skullMat"] = std::move(skullMat);
	mMaterials["sky"] = std::move(sky);
}

void Engine::BuildRenderItems()
{

	if (DefaultObj)
	{
		BUildOneRenderItem(Matrix::MatrixScale(5000.0f, 5000.0f, 5000.0f)
			, "sky", "shapeGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			, RenderLayer::Sky, "sphere");

		BUildOneRenderItem(Matrix::Identity
			, "bricks0", "shapeGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			, RenderLayer::Debug, "quad");
		BUildOneRenderItem(Matrix::MatrixScale(2.0f, 1.0f, 2.0f)*Matrix::MatrixTranslation(0.0f, 0.5f, 0.0f)
			, "bricks0", "shapeGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			, RenderLayer::Opaque, "quad");
		BUildOneRenderItem(Matrix::MatrixScale(0.4f, 0.4f, 0.4f)*Matrix::MatrixTranslation(0.0f, 1.0f, 0.0f)
			, "skullMat", "skullGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			, RenderLayer::Opaque, "skull");

		BUildOneRenderItem(Matrix::Identity
			, "tile0", "skullGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			, RenderLayer::Opaque, "grid", Matrix::MatrixScale(8, 8, 1));
		Matrix brickTexTransform = Matrix::Identity;
		brickTexTransform.ScaleTranslation({ 1.5f, 2.0f, 1.0f });
		UINT objCBIndex = 5;
		for (int i = 0; i < 5; ++i)
		{
			Matrix leftCylWorld = Matrix::MatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f);
			Matrix rightCylWorld = Matrix::MatrixTranslation(+5.0f, 1.5f, -10.0f + i * 5.0f);

			Matrix leftSphereWorld = Matrix::MatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f);
			Matrix rightSphereWorld = Matrix::MatrixTranslation(+5.0f, 3.5f, -10.0f + i * 5.0f);
			BUildOneRenderItem(rightCylWorld
				, "bricks0", "shapeGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
				, RenderLayer::Opaque, "cylinder", brickTexTransform);
			BUildOneRenderItem(leftCylWorld
				, "bricks0", "shapeGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
				, RenderLayer::Opaque, "cylinder", brickTexTransform);
			BUildOneRenderItem(leftSphereWorld
				, "mirror0", "shapeGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
				, RenderLayer::Opaque, "sphere", Matrix::Identity);
			BUildOneRenderItem(rightSphereWorld
				, "mirror0", "shapeGeo", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
				, RenderLayer::Opaque, "sphere", Matrix::Identity);

		}
	}
	
	BUildOneRenderItem(Matrix::Identity
		, "tile0", "Grip", D3D_PRIMITIVE_TOPOLOGY_LINELIST
		, RenderLayer::Opaque, "Grip");
	//mDrawLineControl.Plist.push_back(Vertex());
	//GetGeometry(mDrawLineControl.Plist, mDrawLineControl.LineName);

	//BUildOneRenderItem(Matrix::Identity
	//	, "tile0", mDrawLineControl.LineName, D3D_PRIMITIVE_TOPOLOGY_LINELIST
	//	, RenderLayer::Opaque, mDrawLineControl.LineName);
	////mAllRitems[mAllRitems.size() - 1]->NumFramesDirty = 0;
	//mDrawLineControl.isHaveRenderItem = true;
	//for (int i = 0; i != mFrameResources.size(); i++)
	//{
	//	mFrameResources[i]->ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), (UINT)mAllRitems.size(), true);
	//}
	//isInAddMesh = true;

}

//设置渲染对象

inline void Engine::BUildOneRenderItem(Matrix WordMatrix, string MaterialsName, 
	string mGeometriesName, D3D12_PRIMITIVE_TOPOLOGY PriType, RenderLayer Layer, string SubName, Matrix mTexTransform)
{
	auto skyRitem = std::make_unique<RenderItem>();
	skyRitem->World = WordMatrix;
	skyRitem->TexTransform = mTexTransform;
	skyRitem->ObjCBIndex = RenderConstNum;
	RenderConstNum++;
	skyRitem->Mat = mMaterials[MaterialsName].get();
	skyRitem->Geo = mGeometries[mGeometriesName].get();
	skyRitem->PrimitiveType = PriType;

	skyRitem->IndexCount = skyRitem->Geo->DrawArgs[SubName].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs[SubName].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs[SubName].BaseVertexLocation;
	
	mRitemLayer[(int)Layer].push_back(skyRitem.get());
	
	mAllRitems.push_back(std::move(skyRitem));


}

void Engine::BuildOneLineRenderItem(vector<Vertex> plist)
{
	
}

void Engine::UpdateRenderItems(RenderItem* skyRitem, MeshGeometry* Geo, D3D12_PRIMITIVE_TOPOLOGY PriType,string SubName)
{
	//auto skyRitem = std::make_unique<RenderItem>();

	//RenderConstNum++;
	skyRitem->Geo = Geo;
	skyRitem->PrimitiveType = PriType;

	skyRitem->IndexCount = Geo->DrawArgs[SubName].IndexCount;
	skyRitem->StartIndexLocation = Geo->DrawArgs[SubName].StartIndexLocation;
	skyRitem->BaseVertexLocation = Geo->DrawArgs[SubName].BaseVertexLocation;

	
}

void Engine::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	auto objectCB = mCurrFrameResource->ObjectCB->Resource();

	// For each render item...
	for (size_t i = 0; i < ritems.size(); ++i)
	{
		if (!IsGrip&&ritems[i]->Geo->Name=="Grip")
		{
			continue;
		}
		auto ri = ritems[i];

		cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
		cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex*objCBByteSize;

		cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);

		cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}

void Engine::UpdateShadowTransform(const GameTimer& gt)
{
	Vector4 lightDir = mRotatedLightDirections[0];
	lightDir[3] = 0;
	Vector4 lightPos = -2.0f*mSceneBounds.Radius*lightDir;
	Vector4 targetPos = Vector(mSceneBounds.Center);
	targetPos[3] = 0;
	Vector4 lightUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	Matrix lightView = Matrix::MatrixLookAtP(lightPos, targetPos, lightUp);
	//XMMATRIX lightView = XMMatrixLookAtLH(FXMVECTOR(lightPos.X), targetPos, lightUp);
	mLightPosW=lightPos;

	Vector4 sphereCenterLS;
	sphereCenterLS= lightView.TransformPosition(targetPos);

	// Ortho frustum in light space encloses scene.
	float l = sphereCenterLS.X - mSceneBounds.Radius;
	float b = sphereCenterLS.Y - mSceneBounds.Radius;
	float n = sphereCenterLS.Z - mSceneBounds.Radius;
	float r = sphereCenterLS.X + mSceneBounds.Radius;
	float t = sphereCenterLS.Y + mSceneBounds.Radius;
	float f = sphereCenterLS.Z + mSceneBounds.Radius;
	

	mLightNearZ = n;
	mLightFarZ = f;
	Matrix lightProj = Matrix::MatrixOrtho(l, r, b, t, n, f);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	Matrix T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	Matrix S = lightView * lightProj*T;
	mLightView=lightView;
	mLightProj=lightProj;
	mShadowTransform=S;
}

void Engine::UpdateShadowPassCB(const GameTimer& gt)
{
	Matrix view = mLightView;
	Matrix proj = mLightProj;

	Matrix viewProj = view*proj;
	Matrix invView = view.Inverse();
	Matrix invProj = proj.Inverse();
	Matrix invViewProj = viewProj.Inverse();

	UINT w = mShadowMap->Width();
	UINT h = mShadowMap->Height();


	mShadowPassCB.View=(view).GetTransposed();
	mShadowPassCB.InvView= (invView).GetTransposed();
	mShadowPassCB.Proj=(proj).GetTransposed();
	mShadowPassCB.InvProj= (invProj).GetTransposed();
	mShadowPassCB.ViewProj= (viewProj).GetTransposed();
	mShadowPassCB.InvViewProj= (invViewProj).GetTransposed();



	mShadowPassCB.EyePosW = mLightPosW;
	mShadowPassCB.RenderTargetSize = Vector2D((float)w, (float)h);
	mShadowPassCB.InvRenderTargetSize = Vector2D(1.0f / w, 1.0f / h);
	mMainPassCB.NearZ = mLightNearZ;
	mMainPassCB.FarZ = mLightFarZ;


	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(1, mShadowPassCB);
}

void Engine::DrawSceneToShadowMap()
{
	mCommandList->RSSetViewports(1, &mShadowMap->Viewport());
	mCommandList->RSSetScissorRects(1, &mShadowMap->ScissorRect());

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mShadowMap->Resource(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

	mCommandList->ClearDepthStencilView(mShadowMap->Dsv(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);


	mCommandList->OMSetRenderTargets(0, nullptr, false, &mShadowMap->Dsv());

	auto passCB = mCurrFrameResource->PassCB->Resource();
	D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = passCB->GetGPUVirtualAddress() + 1 * passCBByteSize;
	mCommandList->SetGraphicsRootConstantBufferView(1, passCBAddress);

	mCommandList->SetPipelineState(mPSOs["shadow_opaque"].Get());

	DrawRenderItems(mCommandList.Get(), mRitemLayer[(int)RenderLayer::Opaque]);

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mShadowMap->Resource(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ));
}

void Engine::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 2;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> Engine::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC shadow(
		6, // shaderRegister
		D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressW
		0.0f,                               // mipLODBias
		16,                                 // maxAnisotropy
		D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK);

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp,
		shadow
	};
}