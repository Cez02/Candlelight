#include "Resource.h"

#include "DebugTools.h"

namespace candle::rendering  {


    void Resource::Init(ComPtr<ID3D12Device> device, const size_t bytes, D3D12_HEAP_TYPE type, D3D12_RESOURCE_STATES initialState)
    {
        D3D12_HEAP_PROPERTIES heapProp = {};
        heapProp.Type = type;
        heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProp.CreationNodeMask = 0;
        heapProp.VisibleNodeMask = 0;

        D3D12_RESOURCE_DESC resourceDesc = {};
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Alignment = 0;
        resourceDesc.Width = bytes;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.SampleDesc = { 1, 0 };
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;


        core::DebugTools::AssertAndThrow(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, initialState, 0, IID_PPV_ARGS(&m_Resource)),
            "Failed to create a commited resource!");
    }

    ComPtr<ID3D12Resource> Resource::GetResource() {
        return m_Resource;
    }

    Resource::~Resource() {
        Release();
    }

    void Resource::Release() {

    }
} // candle