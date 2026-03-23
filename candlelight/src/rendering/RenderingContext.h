#pragma once
#include <dxgi1_5.h>
#include <directx/d3d12.h>
#include <wrl/client.h>

#include "../core/architecture/BaseContext.h"

using namespace Microsoft::WRL;

namespace candle::rendering {
    class RenderingContext : public core::BaseContext {
    private:
        ComPtr<ID3D12Device2> m_Device;
        ComPtr<IDXGISwapChain4> m_SwapChain;
        ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;

    public:
        ComPtr<ID3D12Device2> GetDevice() const { return m_Device; }
        void SetDevice(ComPtr<ID3D12Device2> device) { if (!m_Locked) m_Device = device; }

        ComPtr<IDXGISwapChain4> GetSwapChain() const { return m_SwapChain; }
        void SetSwapChain(ComPtr<IDXGISwapChain4> swapchain) { if (!m_Locked) m_SwapChain = swapchain; }

        ComPtr<ID3D12DescriptorHeap> GetRTVDescriptorHeap() const { return m_RTVDescriptorHeap; }
        void SetRTVDescriptorHeap(ComPtr<ID3D12DescriptorHeap> descriptorHeap) { if (!m_Locked) m_RTVDescriptorHeap = descriptorHeap; }
    };
}
