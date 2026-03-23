#include "SwapchainFactory.h"

#include <d3d12.h>
#include <dxgi1_5.h>
#include <wrl.h>

#include "DebugTools.h"

using namespace Microsoft::WRL;

namespace candle::rendering {

    ComPtr<IDXGISwapChain4> SwapChainFactory::CreateSwapChain(RenderingContext ctx, SwapChainSettings settings )
    {
        ComPtr<IDXGISwapChain4> dxgiSwapChain4;
        ComPtr<IDXGIFactory4> dxgiFactory4;
        UINT createFactoryFlags = 0;
#if defined(_DEBUG)
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

        DebugTools::AssertAndThrow(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)), "Failed to create a DXGI factory!");

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = settings.Width;
        swapChainDesc.Height = settings.Height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Stereo = FALSE;
        swapChainDesc.SampleDesc = { 1, 0 };
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = settings.BufferCount;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        // It is recommended to always allow tearing if tearing support is available.
        swapChainDesc.Flags = settings.AllowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

        ComPtr<IDXGISwapChain1> swapChain1;
        DebugTools::AssertAndThrow(dxgiFactory4->CreateSwapChainForHwnd(
            settings.CommandQueue->GetD3D12CommandQueue().Get(),
            ctx.GetHWND(),
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain1),
            "Failed to create a swapchain");

        // Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
        // will be handled manually.
        DebugTools::AssertAndThrow(dxgiFactory4->MakeWindowAssociation(ctx.GetHWND(), DXGI_MWA_NO_ALT_ENTER), "Failed to associate teh window with the swapchain!");

        DebugTools::AssertAndThrow(swapChain1.As(&dxgiSwapChain4), "Failed to cast swapchain as a dxgiSwapChain4!");

        return dxgiSwapChain4;
    }

}
