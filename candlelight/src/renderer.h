#pragma once

#include <wrl.h>
using namespace Microsoft::WRL;

#include <directx/d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <directx/d3dx12.h>

#include <glm/glm.hpp>

#include "command_queue.h"
#include "candlelight_types.h"
#include "render_common.h"
#include "render_pipeline.h"
#include "resource.h"
#include "debug.h"

namespace candle 
{

class Renderer {
private:
    
    // DirectX 12 Objects
    ComPtr<ID3D12Device2> m_Device;
    ComPtr<IDXGISwapChain4> m_SwapChain;
    ComPtr<ID3D12Resource> m_BackBuffers[NUM_OF_FRAMES];
    ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
    UINT m_RTVDescriptorSize;
    UINT m_CurrentBackBufferIndex;

	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT m_ScissorRect;

    // Synchronization objects
    ComPtr<ID3D12Fence> m_Fence;
    uint64_t m_FenceValue = 0;
    uint64_t m_FrameFenceValues[NUM_OF_FRAMES] = {};
    HANDLE m_FenceEvent;

    std::shared_ptr<CommandQueue> m_CommandQueue;

    HDebug m_Debug;

    HRenderPipeline m_RenderPipeline;
    HResource m_DynamicVertexBuffer;

    D3D12_VERTEX_BUFFER_VIEW m_DynamicVBV;

    void DrawWithPipeline(ComPtr<ID3D12GraphicsCommandList2> commandList, ComPtr<ID3D12Resource> backBuffer);

public:

    void Init(HWND hWnd
             ,uint clientWidht
             ,uint clientHeight
             ,bool useWarp);

    void UpdateRenderTargetViews(ComPtr<ID3D12Device2> device
                                ,ComPtr<IDXGISwapChain4> swapChain
                                ,ComPtr<ID3D12DescriptorHeap> descriptorHeap);

    void DrawMesh(const glm::vec3 *vertices, int count);

    void Render();

    ~Renderer();
};

}