#pragma once

#include <memory>
#include <wrl.h>
using namespace Microsoft::WRL;

#include <directx/d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <directx/d3dx12.h>
#include "shader.hpp"


namespace candle 
{

class RenderPipeline{
private:
    ComPtr<ID3D12RootSignature> m_RootSignature;
    ComPtr<ID3D12PipelineState> m_PipelineState;

    HShader m_Shaders[(int)ShaderType::COUNT];

    D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
    ComPtr<ID3D12PipelineState> CreatePipelineState(ComPtr<ID3D12Device> device, HShader vertexShader, HShader pixelShader);
    ComPtr<ID3D12RootSignature> CreateRootSignature(ComPtr<ID3D12Device> device);


public:

    ComPtr<ID3D12RootSignature> GetRootSignature();
    ComPtr<ID3D12PipelineState> GetPipelineState();

    void Init(ComPtr<ID3D12Device> device, HShader vertexShader, HShader pixelShader);
};

typedef std::shared_ptr<RenderPipeline> HRenderPipeline;

}