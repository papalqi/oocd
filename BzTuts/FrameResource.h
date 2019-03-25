#pragma once
#include "stdafx.h"
#include "UploadBuffer.h"
#include "Matrix.h"
#include "Light.h"
#include <unordered_map>
using namespace oocd;

#define MaxLights 16
struct ObjectConstants
{
	Matrix World = Matrix::Identity;
	Matrix TexTransform = Matrix::Identity;
	UINT     MaterialIndex;
	UINT     ObjPad0;
	UINT     ObjPad1;
	UINT     ObjPad2;
};

struct PassConstants
{
	Matrix View = Matrix::Identity;
	Matrix InvView = Matrix::Identity;
	Matrix Proj = Matrix::Identity;
	Matrix InvProj = Matrix::Identity;
	Matrix ViewProj = Matrix::Identity;
	Matrix InvViewProj = Matrix::Identity;
	Matrix ShadowTransform = Matrix::Identity;
	Vector EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	Vector2D RenderTargetSize = { 0.0f, 0.0f };
	Vector2D InvRenderTargetSize = { 0.0f, 0.0f };
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;
	Vector4 AmbientLight  { 0.0f, 0.0f, 0.0f, 1.0f };
	Light Lights[MaxLights];
};

struct MaterialData
{
	Vector4 DiffuseAlbedo { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector FresnelR0  { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.5f;

	
	Matrix MatTransform = Matrix::Identity;
	UINT DiffuseMapIndex = 0;
	UINT NormalMapIndex = 0;
	UINT MaterialPad1;
	UINT MaterialPad2;
};

struct Vertex
{
	Vector Pos;
	Vector Normal;
	Vector2D TexC;
	Vector TangentU;
};

//每一帧渲染的资源

struct FrameResource
{
public:

	FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT materialCount);
	FrameResource(const FrameResource& rhs) = delete;
	FrameResource& operator=(const FrameResource& rhs) = delete;
	~FrameResource();

public:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc;

	std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;

	std::unique_ptr<UploadBuffer<MaterialData>> MaterialBuffer = nullptr;

	UINT64 Fence = 0;
};