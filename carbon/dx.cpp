#include "dx.h"
#include "util.h"
#include "config.h"
#include <comdef.h>
Microsoft::WRL::ComPtr<ID3D12Device> cl_dx_device;
Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
Microsoft::WRL::ComPtr<ID3D12CommandAllocator> r_commandalloc;
const UINT numFrames = 3;
FrameResource frameResources[numFrames];
bool initd3d() {
    #if defined(_DEBUG)
        // Enable the debug layer.
        {
            Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
                logMessage(INFO, "Created debug interface.");
                debugController->EnableDebugLayer();
            }
            else {
                logError(CRITICAL, "Failed to create d3d12 debug interface.");
                return false;
            }
        }
    #endif

    Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(hr)) {
        logError(CRITICAL, "Failed to create DXGI factory!");
        return false;
    }

    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        hr = adapter->GetDesc1(&desc);
        if (FAILED(hr))
            //need this
            continue;
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            //skip software adapters 
            continue;
        }

        if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0,_uuidof(ID3D12Device), nullptr))) {
            char devId[sizeof(desc.DeviceId) * 5];
            sprintf_s(devId, "%d", desc.DeviceId);
            static char logStr[33] = "Found D3D12 compatible adapter: ";
            logMessage(INFO,strcat(logStr, devId));
            break;
        }
    }

    #if !defined(NDEBUG)
        if (!adapter)
        {
            if (FAILED(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&adapter))))
            {
                adapter.Reset();
            }
        }
    #endif

    
    if (!adapter) {
        logError(CRITICAL, "Failed to find adapter with D3D12 support!");
        return false;
    }

    
    hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&cl_dx_device));
    if (FAILED(hr)) {
        logError(CRITICAL, "Failed to create D3D12 device!");
        return false;
    }

    static const D3D_FEATURE_LEVEL s_featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D12_FEATURE_DATA_FEATURE_LEVELS featLevels =
    {
        _countof(s_featureLevels), s_featureLevels, D3D_FEATURE_LEVEL_11_0
    };

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    hr = cl_dx_device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS,
        &featLevels, sizeof(featLevels));
    if (SUCCEEDED(hr))
    {
        featureLevel = featLevels.MaxSupportedFeatureLevel;
    }

    logMessage(INFO, "Setting up swapchain & command queue");

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;

    if (FAILED(cl_dx_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)))) {
        //std::cerr << "Failed to create command queue." << std::endl;
        logError(CRITICAL, "Failed to create command queue.");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = confWidth;
    swapChainDesc.Height = confHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1; // no multisampling
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 3; // triple buffered
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = 0;

    if (FAILED(dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), cl_hwnd, &swapChainDesc, nullptr, nullptr, &swapChain))) {
        logError(CRITICAL, "Failed to create swapchain");
        return false;
    }
    logMessage(INFO, "Set up swapchain");
    
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = 3;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    logMessage(INFO, "Setting up RTV descriptor heap");
    if (FAILED(cl_dx_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap)))) {
        logError(CRITICAL, "Failed to create RTV descriptor heap");
        return false;
    }
    logMessage(INFO, "Set up RTV descriptor heap");

    // god help me this is going to suck
    for (UINT i = 0; i < numFrames; ++i) {
        //logMessage(INFO, "funny");
        D3D12_DESCRIPTOR_HEAP_DESC frameRtvHeapDesc = {};
        frameRtvHeapDesc.NumDescriptors = 1; // Assuming only one render target
        frameRtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        frameRtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        if (FAILED(cl_dx_device->CreateDescriptorHeap(&frameRtvHeapDesc, IID_PPV_ARGS(&frameResources[i].rtvDescriptorHeap)))) {
            std::cerr << "Failed to create RTV descriptor heap for frame " << i << std::endl;
            return false;
        }

        // Create command allocator
        if (FAILED(cl_dx_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frameResources[i].commandAllocator)))) {
            std::cerr << "Failed to create command allocator for frame " << i << std::endl;
            return false;
        }

        // Create command list
        if (FAILED(cl_dx_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, frameResources[i].commandAllocator.Get(), nullptr, IID_PPV_ARGS(&frameResources[i].commandList)))) {
            std::cerr << "Failed to create command list for frame " << i << std::endl;
            return false;
        }
    }

    if (FAILED(cl_dx_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&r_commandalloc)))) {
        logError(HIGH, "Failed to create global command allocator");
        return false;
    }

    return true;
}

bool initAssets() {
    return false;
}