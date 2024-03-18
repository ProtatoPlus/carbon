#pragma once
#include "main.h"
#include "d3d12.h"
#include <dxgi1_4.h>
#include "d3d12sdklayers.h"
#include "combaseapi.h"
#include "wlclient.h"
#include <wrl/client.h>
#include "logging.h"
#include <iostream>
bool initd3d();
bool initSwapChain();
extern Microsoft::WRL::ComPtr<ID3D12Device> cl_dx_device;
extern Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
extern Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
extern Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
extern const UINT numFrames;
extern Microsoft::WRL::ComPtr<ID3D12CommandAllocator> r_commandalloc;

struct FrameResource {
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTarget;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
};

bool initAssets();

extern FrameResource frameResources[];