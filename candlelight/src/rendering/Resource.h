#pragma once

#include <d3d12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

#include <memory>

namespace candle::rendering  {

    class Resource {
    private:

        ComPtr<ID3D12Resource> m_Resource;

    public:

        void Init(ComPtr<ID3D12Device> device, const size_t size, D3D12_HEAP_TYPE type, D3D12_RESOURCE_STATES initialState);
        ComPtr<ID3D12Resource> GetResource();

        void Release();

        ~Resource();
    };

    typedef std::shared_ptr<Resource> HResource;

} // candle

