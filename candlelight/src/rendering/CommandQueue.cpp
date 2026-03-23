#include "CommandQueue.h"

#include <windows.h>
#include <wrl.h>

#include "DebugTools.h"
#include "RenderingContext.h"
using namespace Microsoft::WRL;

#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

#include "WindowsUtils.h"


using namespace candle::rendering;
using namespace candle::core;


CommandQueue::CommandQueue(RenderingContext ctx, D3D12_COMMAND_LIST_TYPE type)
    : BaseObject(ctx)
    , m_CommandListType(type)
    , m_d3d12Device(ctx.GetDevice())
    , m_FenceValue(0){

    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = type;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    DebugTools::AssertAndThrow(m_d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12CommandQueue)), "Creating command queue failed!");
    DebugTools::AssertAndThrow(m_d3d12Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12Fence)), "Creating a fence failed!");

    m_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    assert(m_FenceEvent && "Failed to create fence event handle.");
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandQueue::CreateCommandAllocator()
{
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
    DebugTools::AssertAndThrow(m_d3d12Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)), "Creating command allocator failed!");

    return commandAllocator;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CommandQueue::CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator)
{
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;
    DebugTools::AssertAndThrow(m_d3d12Device->CreateCommandList(0, m_CommandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)), "Creating command list failed!");

    return commandList;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CommandQueue::GetCommandList()
{
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;

    if ( !m_CommandAllocatorQueue.empty() && IsFenceComplete(m_CommandAllocatorQueue.front().fenceValue))
    {
        commandAllocator = m_CommandAllocatorQueue.front().commandAllocator;
        m_CommandAllocatorQueue.pop();

        DebugTools::AssertAndThrow(commandAllocator->Reset(), "Command allocator reset failed!");
    }
    else
    {
        commandAllocator = CreateCommandAllocator();
    }

    if (!m_CommandListQueue.empty())
    {
        commandList = m_CommandListQueue.front();
        m_CommandListQueue.pop();

        DebugTools::AssertAndThrow(commandList->Reset(commandAllocator.Get(), nullptr), "Command allocator reset failed!");
    }
    else
    {
        commandList = CreateCommandList(commandAllocator);
    }

    // Associate the command allocator with the command list so that it can be
    // retrieved when the command list is executed.
    DebugTools::AssertAndThrow(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()), "Setting private data interface failed!");

    return commandList;
}

uint64_t CommandQueue::ExecuteCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList)
{
    ID3D12CommandAllocator* commandAllocator;
    UINT dataSize = sizeof(commandAllocator);
    DebugTools::AssertAndThrow(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator), "Getting private data failed!");

    commandList->Close();

    ID3D12CommandList* const ppCommandLists[] = {
        commandList.Get()
    };

    m_d3d12CommandQueue->ExecuteCommandLists(1, ppCommandLists);
    uint64_t fenceValue = Signal();

    m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
    m_CommandListQueue.push(commandList);

    // The ownership of the command allocator has been transferred to the ComPtr
    // in the command allocator queue. It is safe to release the reference 
    // in this temporary COM pointer here.
    commandAllocator->Release();

    return fenceValue;
}

uint64_t CommandQueue::Signal()
{
    uint64_t fenceValueForSignal = ++m_FenceValue;
    DebugTools::AssertAndThrow(m_d3d12CommandQueue->Signal(m_d3d12Fence.Get(), fenceValueForSignal), "Sending the command queue signal failed!");

    return fenceValueForSignal;
}

bool candle::CommandQueue::IsFenceComplete(uint64_t fenceValue) const {
    return m_d3d12Fence->GetCompletedValue() >= fenceValue;
}

void candle::CommandQueue::WaitForFenceValue(uint64_t fenceValue) const {
    if(!IsFenceComplete(fenceValue)){
        m_d3d12Fence->SetEventOnCompletion(fenceValue, m_FenceEvent);
        ::WaitForSingleObject(m_FenceEvent, DWORD_MAX);
    }
}

void candle::CommandQueue::Flush()
{
    WaitForFenceValue(Signal());
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> candle::CommandQueue::GetD3D12CommandQueue() const
{
    return m_d3d12CommandQueue;
}
