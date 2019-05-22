#pragma once
#include<string>
#include "stdafx.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix.h"
using namespace std;
using namespace oocd;
using namespace Microsoft;
extern const int gNumFrameResources;
class d3dUtil
{
public:

	static bool IsKeyDown(int vkeyCode);

	static std::string ToString(HRESULT hr);

	static UINT CalcConstantBufferByteSize(UINT byteSize)
	{
		
		return (byteSize + 255) & ~255;
	}

	static Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::wstring& filename);

	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

	static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target);
};
struct Material
{
	std::string Name;

	int MatCBIndex = -1;

	int DiffuseSrvHeapIndex = -1;
	int NormalSrvHeapIndex = -1;

	int NumFramesDirty = gNumFrameResources;

	Vector4 DiffuseAlbedo { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector FresnelR0 { 0.01f, 0.01f, 0.01f };
	float Roughness = .25f;
	Matrix MatTransform = Matrix::Identity;
};
struct Light
{
	Vector Strength = { 0.5f, 0.5f, 0.5f };
	float FalloffStart = 1.0f;                         
	Vector Direction = { 0.0f, -1.0f, 0.0f };
	float FalloffEnd = 10.0f;                          
	Vector Position = { 0.0f, 0.0f, 0.0f };  
	float SpotPower = 64.0f;                            
};
struct Texture
{
	// Unique material name for lookup.
	std::string Name;

	std::wstring Filename;

	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;
};