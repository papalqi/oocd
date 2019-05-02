﻿#pragma once
#include"EngineBase.h"
#include <unordered_map>
#include "GCMath.h"
#include "Matrix.h"
#include "FrameResource.h"
#include "Mesh.h"
#include "Light.h"
#include <array>
#include <fstream>
#include "TextureLoad.h"
#include<DirectXColors.h>
#include "GeometryGenerator.h"
#include "Camera.h"
#include"ShadowMap.h"
using namespace std;
using namespace oocd;
using namespace DirectX;
using namespace Microsoft::WRL;

struct RenderItem
{
	RenderItem() = default;
	RenderItem(const RenderItem& rhs) = delete;

	Matrix World = Matrix::Identity;

	Matrix TexTransform = Matrix::Identity;

	//这个数据是否是脏的
	int NumFramesDirty = gNumFrameResources;

	//GPU的常量索引
	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};
 class  Engine : public EngineBase
{
public:
	Engine(HINSTANCE hInstance);
	//Engine(HWND WHWND);
	Engine(const Engine& rhs) = delete;			  //不允许使用复制构造函数
	Engine& operator=(const Engine& rhs) = delete;//不允许使用赋值构造函数
	~Engine();
	GameTimer*						GetTimer() { return &mTimer; };
	//进行Engine初始化

	void						RunWithQTInOne();
	virtual bool				Initialize()								override;
	virtual bool				Initialize(HWND Bwindows)					override;
	inline void					SetWidth(int inWidth) { mClientWidth = inWidth; };
	inline void					SetHeight(int inHeight) { mClientHeight = inHeight; };
	virtual void				OnResize()									override;
	void						BuildGrip();

public:

	virtual void				Update(const GameTimer& gt)					override;
	virtual void				Draw(const GameTimer& gt)					override;

	virtual void				OnMouseDown(WPARAM btnState, int x, int y)	override;
	virtual void				OnMouseUp(WPARAM btnState, int x, int y)	override;
	virtual void				OnMouseMove(WPARAM btnState, int x, int y)	override;

	void						OnKeyboardInput(const GameTimer& gt);		//相应键盘
	void						AnimateMaterials(const GameTimer& gt);
	void						UpdateObjectCBs(const GameTimer& gt);		//更新物体的CBV
	void						UpdateMaterialBuffer(const GameTimer& gt);
	void						UpdateMainPassCB(const GameTimer& gt);
	void						UpdateShadowPassCB(const GameTimer& gt);
	void						LoadTextures();				//加载texture
	void						BuildRootSignature();		//建立RootSignature
	void						BuildDescriptorHeaps();		//建立描述符堆
	void						BuildShadersAndInputLayout();//创建shader和InputLayout
	void						BuildShapeGeometry();		//建立几何数据
	void						BuildSkullGeometry();		//头骨信息
	void						BuildOBJMesh(string File, string FliePath);				//建立OBJ物体
	void						BuildPSOs();				//建立PSO
	void						BuildFrameResources();
	void						BuildMaterials();			//建立材质
	void						BuildRenderItems();			//设置渲染对象
	void						DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const vector<RenderItem*>& ritems);
	void						UpdateShadowTransform(const GameTimer& gt);
	void						DrawSceneToShadowMap();
	void						CreateRtvAndDsvDescriptorHeaps()override;

	array<const CD3DX12_STATIC_SAMPLER_DESC, 7>					GetStaticSamplers();//获取静态采样器

private:
	std::unique_ptr<ShadowMap>									mShadowMap;
	Camera														mCamera;
	ComPtr<ID3D12DescriptorHeap>								mSrvDescriptorHeap = nullptr;	//SRV描述符堆
	ComPtr<ID3D12RootSignature>									mRootSignature = nullptr;		//根描述符
	FrameResource*												mCurrFrameResource = nullptr;	
	int															mCurrFrameResourceIndex = 0;	//当前的渲染帧索引
	PassConstants												mMainPassCB;		
	PassConstants												mShadowPassCB;
	POINT														mLastMousePos;
	UINT														mCbvSrvDescriptorSize = 0;		//cbv的增量大小
	UINT														mSkyTexHeapIndex = 0;			//天空的索引
	unordered_map<string, ComPtr<ID3D12PipelineState>>			mPSOs;
	unordered_map<string, ComPtr<ID3DBlob>>						mShaders;
	unordered_map<string, unique_ptr<Material>>					mMaterials;
	unordered_map<string, unique_ptr<MeshGeometry>>				mGeometries;
	unordered_map<string, unique_ptr<Texture>>					mTextures;
	vector<D3D12_INPUT_ELEMENT_DESC>							mInputLayout;					//输入的格式
	vector<RenderItem*>											mRitemLayer[(int)RenderLayer::Count];
	vector<unique_ptr<FrameResource>>						    mFrameResources;
	vector<unique_ptr<RenderItem>>								mAllRitems;
private:
	CD3DX12_GPU_DESCRIPTOR_HANDLE mNullSrv;
	DirectX::BoundingSphere mSceneBounds;
	float mLightNearZ = 0.0f;
	float mLightFarZ = 0.0f;
	Vector mLightPosW;
	Matrix mLightView = Matrix::Identity;
	Matrix mLightProj = Matrix::Identity;
	Matrix mShadowTransform = Matrix::Identity;



	UINT mShadowMapHeapIndex = 0;

	UINT mNullCubeSrvIndex = 0;
	UINT mNullTexSrvIndex = 0;


	float mLightRotationAngle = 0.0f;
	Vector mBaseLightDirections[3] = {
		Vector(0.57735f, -0.57735f, 0.57735f),
		Vector(-0.57735f, -0.57735f, 0.57735f),
		Vector(0.0f, -0.707f, -0.707f)
	};
	Vector mRotatedLightDirections[3];

};
