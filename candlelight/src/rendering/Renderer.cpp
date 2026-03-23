#include "Renderer.h"

#include <windows.h>

#pragma comment(lib, "dxgi")

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

// D3D12 extension library.
#include <directx/d3dx12.h>

#include "SwapChainFactory.h"

#include <chrono>

#include "DebugTools.h"
#include "algorithm/memory/MemoryAlgorithms.h"

using namespace candle;
using namespace candle::core;
using namespace candle::rendering;

ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp)
{
    ComPtr<IDXGIFactory4> dxgiFactory;
    UINT createFactoryFlags = 0;
#if defined(_DEBUG)
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

    DebugTools::AssertAndThrow(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)), "Failed to create a DXGI factory!");

    ComPtr<IDXGIAdapter1> dxgiAdapter1;
    ComPtr<IDXGIAdapter4> dxgiAdapter4;

    if (useWarp)
    {
        DebugTools::AssertAndThrow(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)), "Failed enum warp adapter!");
        DebugTools::AssertAndThrow(dxgiAdapter1.As(&dxgiAdapter4), "Failed to cast dxgiAdapter1 as dxgiAdapter4!");
    }
    else
    {
        SIZE_T maxDedicatedVideoMemory = 0;
        for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
            dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

            // Check to see if the adapter can create a D3D12 device without actually 
            // creating it. The adapter with the largest dedicated video memory
            // is favored.
            if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
                SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(), 
                    D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) && 
                dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory )
            {
                maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
                DebugTools::AssertAndThrow(dxgiAdapter1.As(&dxgiAdapter4), "Failed to cast dxgiAdapter1 as dxgiAdapter4!");
            }
        }
    }

    return dxgiAdapter4;
}

ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> adapter)
{
    ComPtr<ID3D12Device2> d3d12Device2;
    DebugTools::AssertAndThrow(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)), "Failed to create a D3D12 device!");
    
    // Enable debug messages in debug mode.
#if defined(_DEBUG)
    ComPtr<ID3D12InfoQueue> pInfoQueue;
    if (SUCCEEDED(d3d12Device2.As(&pInfoQueue)))
    {
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, FALSE);

        // Suppress whole categories of messages
        //D3D12_MESSAGE_CATEGORY Categories[] = {};

        // Suppress messages based on their severity level
        D3D12_MESSAGE_SEVERITY Severities[] =
        {
            D3D12_MESSAGE_SEVERITY_INFO
        };

        // Suppress individual messages by their ID
        D3D12_MESSAGE_ID DenyIds[] = {
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
        };

        D3D12_INFO_QUEUE_FILTER NewFilter = {};
        //NewFilter.DenyList.NumCategories = _countof(Categories);
        //NewFilter.DenyList.pCategoryList = Categories;
        NewFilter.DenyList.NumSeverities = _countof(Severities);
        NewFilter.DenyList.pSeverityList = Severities;
        NewFilter.DenyList.NumIDs = _countof(DenyIds);
        NewFilter.DenyList.pIDList = DenyIds;

        DebugTools::AssertAndThrow(pInfoQueue->PushStorageFilter(&NewFilter), "Failed push the storage filter!");
    }
#endif

    return d3d12Device2;
}

bool CheckTearingSupport()
{
    BOOL allowTearing = FALSE;

    // Rather than create the DXGI 1.5 factory interface directly, we create the
    // DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
    // graphics debugging tools which will not support the 1.5 factory interface 
    // until a future update.
    ComPtr<IDXGIFactory4> factory4;
    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
    {
        ComPtr<IDXGIFactory5> factory5;
        if (SUCCEEDED(factory4.As(&factory5)))
        {
            if (FAILED(factory5->CheckFeatureSupport(
                DXGI_FEATURE_PRESENT_ALLOW_TEARING, 
                &allowTearing, sizeof(allowTearing))))
            {
                allowTearing = FALSE;
            }
        }
    }

    return allowTearing == TRUE;
}

ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device2> device,
    D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors)
{
    ComPtr<ID3D12DescriptorHeap> descriptorHeap;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = numDescriptors;
    desc.Type = type;

    DebugTools::AssertAndThrow(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)), "Failed create a descriptor heap!");

    return descriptorHeap;
}

void Renderer::UpdateRenderTargetViews(ComPtr<ID3D12Device2> device,
    ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap)
{
    auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

    for (int i = 0; i < NUM_OF_FRAMES; ++i)
    {
        ComPtr<ID3D12Resource> backBuffer;
        DebugTools::AssertAndThrow(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)), "Failed to get a swapchain buffer!");

        device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

        m_BackBuffers[i] = backBuffer;

        rtvHandle.Offset(rtvDescriptorSize);
    }
}

void Renderer::DrawMesh(const glm::vec3 *vertices, int count)
{
    
}

ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device,
    D3D12_COMMAND_LIST_TYPE type)
{
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    DebugTools::AssertAndThrow(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)), "Failed to create a command allocator!");

    return commandAllocator;
}

ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> device,
    ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)
{
    ComPtr<ID3D12GraphicsCommandList> commandList;
    DebugTools::AssertAndThrow(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)), "Failed to create a command list!");
    
    DebugTools::AssertAndThrow(commandList->Close(), "Failed to close the command list!");

    return commandList;
}

ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device)
{
    ComPtr<ID3D12Fence> fence;

    DebugTools::AssertAndThrow(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)), "Failed to create a fence!");

    return fence;
}

HANDLE CreateEventHandle()
{
    HANDLE fenceEvent;
    
    fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    assert(fenceEvent && "Failed to create fence event.");

    return fenceEvent;
}

HRenderPipeline CreateRenderPipeline(ComPtr<ID3D12Device> device, RenderingContext renderingContext)
{
    // todo: Replace hardcoded shaders...

    HShader vertexShader = std::make_shared<Shader>();
    vertexShader->Init("./shaders/default_shader.vs.hlsl", ShaderType::VERTEX);

    HShader pixelShader = std::make_shared<Shader>();
    pixelShader->Init("./shaders/default_shader.ps.hlsl", ShaderType::PIXEL);

    HRenderPipeline pipeline = std::make_shared<RenderPipeline>(renderingContext);
    pipeline->Init(device, vertexShader, pixelShader);

    return pipeline;
}



HResource CreateDynamicVertexBuffer(ComPtr<ID3D12Device> device){

    HResource resource = std::make_shared<Resource>();

    resource->Init(device, memory::Kbs2Bytes(16), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
    resource->GetResource()->SetName(L"Dynamic vertex buffer");


    // add test triangle

    void* dest = nullptr;

    resource->GetResource()->Map(0, 0, &dest);

    // modify buffer HERE

    float data[] = {
        -0.5f, -0.5f, 0.0f,      // vertex 1
        1.0f, 0.0f, 0.0f, 1.0f, // color

        0.0f, 0.5f, 0.0f,      // vertex 2
        0.0f, 1.0f, 0.0f, 1.0f, // color
    
        0.5f, -0.5f, 0.0f,      // vertex 3
        0.0f, 0.0f, 1.0f, 1.0f, // color
    };

	memcpy(dest, data, sizeof(data));

    resource->GetResource()->Unmap(0, 0);

    return resource;
}

void SetupVBV(ComPtr<ID3D12Device> device, HResource resource, D3D12_VERTEX_BUFFER_VIEW &vertexBufferView){

    vertexBufferView.BufferLocation = resource->GetResource()->GetGPUVirtualAddress();
    vertexBufferView.StrideInBytes = sizeof(glm::vec3) + sizeof(glm::vec4); // todo: update with custom mesh API
    vertexBufferView.SizeInBytes = memory::Kbs2Bytes(16);
}

HD3D12Debug CreateDebug(){
    HD3D12Debug hDebug = D3D12Debug::CreateInstance();
    hDebug->Enable();

    return hDebug;
}

uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
    uint64_t& fenceValue)
{
    uint64_t fenceValueForSignal = ++fenceValue;
    DebugTools::AssertAndThrow(commandQueue->Signal(fence.Get(), fenceValueForSignal), "Failed to cast dxgiAdapter1 as dxgiAdapter4!");

    return fenceValueForSignal;
}

void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent,
    std::chrono::milliseconds duration = std::chrono::milliseconds::max() )
{
    if (fence->GetCompletedValue() < fenceValue)
    {
        DebugTools::AssertAndThrow(fence->SetEventOnCompletion(fenceValue, fenceEvent), "Failed to set event on completion of fence!");
        ::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
    }
}

void Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
    uint64_t& fenceValue, HANDLE fenceEvent )
{
    uint64_t fenceValueForSignal = Signal(commandQueue, fence, fenceValue);
    WaitForFenceValue(fence, fenceValueForSignal, fenceEvent);
}

void Renderer::InitDeviceAndAdapters(RendererInitSettings settings) {
#ifdef _DEBUG
    m_Debug = CreateDebug();
#endif

    ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetAdapter(settings.UseWarp);

    m_RenderingContext.SetDevice(CreateDevice(dxgiAdapter4));

    m_Debug->SetDevice(m_RenderingContext.GetDevice());

    DebugTools::AssertAndThrow(m_RenderingContext.GetDevice()->GetDeviceRemovedReason(), "The device was removed!");

    m_CommandQueue = std::make_shared<CommandQueue>(m_RenderingContext, D3D12_COMMAND_LIST_TYPE_DIRECT);
}

void Renderer::InitFrameObjects(RendererInitSettings settings) {
    SwapChainSettings swapChainSettings = {
        settings.ClientWidth,
        settings.ClientHeight,
        NUM_OF_FRAMES,
        CheckTearingSupport(),
        m_CommandQueue
    };
    auto swapChain = SwapChainFactory::CreateSwapChain(m_RenderingContext,swapChainSettings);
    m_RenderingContext.SetSwapChain(swapChain);

    m_CurrentBackBufferIndex = m_RenderingContext.GetSwapChain()->GetCurrentBackBufferIndex();

    m_RenderingContext.SetRTVDescriptorHeap(CreateDescriptorHeap(m_RenderingContext.GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, NUM_OF_FRAMES));
    m_RTVDescriptorSize = m_RenderingContext.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    m_Viewport.TopLeftX = 0.0f;
    m_Viewport.TopLeftY = 0.0f;
    m_Viewport.Width = static_cast<float>(settings.ClientWidth);
    m_Viewport.Height = static_cast<float>(settings.ClientHeight);
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;

    m_ScissorRect.left = 0;
    m_ScissorRect.right = static_cast<float>(settings.ClientWidth);
    m_ScissorRect.top = 0;
    m_ScissorRect.bottom = static_cast<float>(settings.ClientHeight);

    m_RenderPipeline = CreateRenderPipeline(m_RenderingContext.GetDevice(), m_RenderingContext);

    m_DynamicVertexBuffer = CreateDynamicVertexBuffer(m_RenderingContext.GetDevice());
    SetupVBV(m_RenderingContext.GetDevice(), m_DynamicVertexBuffer, m_DynamicVBV);

    UpdateRenderTargetViews(m_RenderingContext.GetDevice(), m_RenderingContext.GetSwapChain(), m_RenderingContext.GetRTVDescriptorHeap());
}

void Renderer::Init(RendererInitSettings settings){

    m_RenderingContext.UnlockContext();

    InitDeviceAndAdapters(settings);
    InitFrameObjects(settings);

    m_RenderingContext.LockContext();
}


void Renderer::DrawWithPipeline(ComPtr<ID3D12GraphicsCommandList2> commandList, ComPtr<ID3D12Resource> backBuffer) {

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RenderingContext.GetRTVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
                                      m_CurrentBackBufferIndex, m_RTVDescriptorSize);
    commandList->OMSetRenderTargets(1, &rtv, false, nullptr);

	commandList->RSSetViewports(1, &m_Viewport);
	commandList->RSSetScissorRects(1, &m_ScissorRect);

    commandList->SetGraphicsRootSignature(m_RenderPipeline->GetRootSignature().Get());
    commandList->SetPipelineState(m_RenderPipeline->GetPipelineState().Get());

    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->IASetVertexBuffers(0, 1, &m_DynamicVBV);

    // draw call
    commandList->DrawInstanced(3, 1, 0, 0);
}

Renderer::Renderer(const BaseContext &context)
    : BaseObject(context)
    , m_RTVDescriptorSize(0)
    , m_CurrentBackBufferIndex(0)
    , m_Viewport()
    , m_ScissorRect()
    , m_FenceEvent(nullptr)
    , m_DynamicVBV() {
    memcpy(&m_RenderingContext, &context, sizeof(context));
}


void Renderer::Render() {
    auto backBuffer = m_BackBuffers[m_CurrentBackBufferIndex];
    auto commandList = m_CommandQueue->GetCommandList();

    // Clear the render target.
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer.Get(),
            D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

        commandList->ResourceBarrier(1, &barrier);
     
        FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RenderingContext.GetRTVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
            m_CurrentBackBufferIndex, m_RTVDescriptorSize);

        commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    }


    // use the render pipeline

    DrawWithPipeline(commandList, backBuffer);


    // Present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        commandList->ResourceBarrier(1, &barrier);

        m_CommandQueue->ExecuteCommandList(commandList);

        UINT syncInterval = 1;
        UINT presentFlags = 0;
        DebugTools::AssertAndThrow(m_RenderingContext.GetSwapChain()->Present(syncInterval, presentFlags));

        m_CurrentBackBufferIndex = m_RenderingContext.GetSwapChain()->GetCurrentBackBufferIndex();

        m_CommandQueue->WaitForFenceValue(m_FrameFenceValues[m_CurrentBackBufferIndex]);
    }
}

Renderer::~Renderer()
{
    m_CommandQueue->Flush();

    m_RenderingContext.UnlockContext();

    m_RenderPipeline = nullptr;
    m_RenderingContext.SetRTVDescriptorHeap(nullptr);
    m_RenderingContext.SetSwapChain(nullptr);
    m_CommandQueue = nullptr;
    m_DynamicVertexBuffer = nullptr;
    m_RenderingContext.SetDevice(nullptr);

    m_RenderingContext.LockContext();
}
