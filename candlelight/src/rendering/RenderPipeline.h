#pragma once

#include <memory>
#include <wrl.h>

#include "RenderingContext.h"
#include "architecture/BaseObject.h"
using namespace Microsoft::WRL;

#include <directx/d3d12.h>
#include "Shader.hpp"


namespace candle::rendering
{
    class RenderPipeline;
    typedef std::shared_ptr<RenderPipeline> HRenderPipeline;

    class RenderPipeline : core::BaseObject<RenderingContext>{
    private:
        ComPtr<ID3D12RootSignature> m_RootSignature;
        ComPtr<ID3D12PipelineState> m_PipelineState;

        HShader m_Shaders[static_cast<int>(ShaderType::COUNT)];

        D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
        ComPtr<ID3D12PipelineState> CreatePipelineState(ComPtr<ID3D12Device> device, HShader vertexShader, HShader pixelShader);
        ComPtr<ID3D12RootSignature> CreateRootSignature(ComPtr<ID3D12Device> device);

    public:

        RenderPipeline(RenderingContext ctx);

        ComPtr<ID3D12RootSignature> GetRootSignature();
        ComPtr<ID3D12PipelineState> GetPipelineState();

        void Init(ComPtr<ID3D12Device> device, HShader vertexShader, HShader pixelShader);
    };

}