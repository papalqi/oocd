#pragma once
//实现阴影
#include "d3dUtil.h"

class ShadowMap
{
public:
	ShadowMap(ID3D12Device* device,
		UINT width, UINT height);

	//不允许使用复制构造
	ShadowMap(const ShadowMap& rhs) = delete;
	//不允许使用赋值构造
	ShadowMap& operator=(const ShadowMap& rhs) = delete;
	~ShadowMap() = default;

   
	inline UINT								Width()const { return mWidth; };
	inline UINT								Height()const { return mHeight; };
	ID3D12Resource*							Resource() { return mShadowMap.Get(); };
	inline CD3DX12_GPU_DESCRIPTOR_HANDLE	Srv()const { return mhGpuSrv; };
	inline CD3DX12_CPU_DESCRIPTOR_HANDLE	Dsv()const { return mhCpuDsv; };

	inline D3D12_VIEWPORT					Viewport()const { return mViewport; };
	inline D3D12_RECT						ScissorRect()const { return mScissorRect; };

	void		BuildDescriptors(
				CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuSrv,
				CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuSrv,
				CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDsv);

	void		OnResize(UINT newWidth, UINT newHeight);

private:
	void		BuildDescriptors();
	void		BuildResource();

private:

	ID3D12Device*		md3dDevice = nullptr;

	D3D12_VIEWPORT		mViewport;
	D3D12_RECT			mScissorRect;

	UINT				mWidth = 0;
	UINT				mHeight = 0;
	DXGI_FORMAT			mFormat = DXGI_FORMAT_R24G8_TYPELESS;

	CD3DX12_CPU_DESCRIPTOR_HANDLE			mhCpuSrv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE			mhGpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE			mhCpuDsv;

	Microsoft::WRL::ComPtr<ID3D12Resource>	mShadowMap = nullptr;
};

