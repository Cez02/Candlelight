#pragma once

#include <wrl.h>

#include "candlelight_types.h"
#include "RenderingContext.h"
#include "architecture/BaseObject.h"
#include "renderers/MeshRenderer.h"
using namespace Microsoft::WRL;

#include <directx/d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <directx/d3dx12.h>

#include <glm/glm.hpp>

#include "CommandQueue.h"
#include "RenderPipeline.h"
#include "Resource.h"
#include "D3D12Debug.h"

namespace candle::rendering
{
    struct RendererInitSettings {
    public:
        uint ClientWidth;
        uint ClientHeight;
        bool UseWarp;
    };

    class Renderer : public BaseObject<BaseContext> {
    public:
        static constexpr uint8_t NUM_OF_FRAMES = 3;

    private:
        // Contexts
        RenderingContext m_RenderingContext;

        // Data
        // DirectX 12 Objects
        ComPtr<ID3D12Resource> m_BackBuffers[NUM_OF_FRAMES];
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

        HD3D12Debug m_Debug;

        HRenderPipeline m_RenderPipeline;

        HMeshRenderer m_ExampleMesh;


        // Methods
        void DrawWithPipeline(ComPtr<ID3D12GraphicsCommandList2> commandList, ComPtr<ID3D12Resource> backBuffer);

        void InitDeviceAndAdapters(RendererInitSettings settings);
        void InitFrameObjects(RendererInitSettings settings);

    public:
        explicit Renderer(const BaseContext &context);

        void Init(RendererInitSettings settings);

        void UpdateRenderTargetViews(ComPtr<ID3D12Device2> device
                                    ,ComPtr<IDXGISwapChain4> swapChain
                                    ,ComPtr<ID3D12DescriptorHeap> descriptorHeap);

        void DrawMesh(const glm::vec3 *vertices, int count);

        void Render();

        ~Renderer();

        BaseContext GetContext() override { return static_cast<BaseContext>(m_RenderingContext); }
    };

    typedef std::shared_ptr<Renderer> HRenderer;

}