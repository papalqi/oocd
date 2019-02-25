#pragma once
#include "d3dx12.h"
class Texture
{
	ID3D12DescriptorHeap* mainDescriptorHeap;
	ID3D12Resource* textureBuffer; // the resource heap containing our texture
	ID3D12Resource* textureBuffer1; // the resource heap containing our texture

	int LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename, int &bytesPerRow);

	DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
	WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
	int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
	ID3D12Resource* textureBufferUploadHeap;
};