#pragma once
#include <d3d12.h>
#include <dxgi1_5.h>
#include <wrl/client.h>

#include "CommandQueue.h"
#include "RenderingContext.h"

using namespace Microsoft::WRL;

namespace candle::rendering {

    struct SwapChainSettings {
    public:
        uint32_t Width;
        uint32_t Height;
        uint32_t BufferCount;
        bool AllowTearing;

        HCommandQueue CommandQueue;
    };

    class SwapChainFactory {
    public:
        static ComPtr<IDXGISwapChain4> CreateSwapChain(const RenderingContext ctx, SwapChainSettings settings);
    };
}
