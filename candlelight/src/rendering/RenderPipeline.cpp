#include "RenderPipeline.h"

#include <glm/glm.hpp>

#include "DebugTools.h"

namespace candle::rendering
{

    ComPtr<ID3D12RootSignature> RenderPipeline::CreateRootSignature(ComPtr<ID3D12Device> device){
        D3D12_ROOT_PARAMETER rootParameters[1];
        rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        rootParameters[0].Descriptor.ShaderRegister = 0;
        rootParameters[0].Descriptor.RegisterSpace = 0;
        rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

        D3D12_VERSIONED_ROOT_SIGNATURE_DESC rsDesc = { };

        rsDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
        rsDesc.Desc_1_0.pParameters = rootParameters;
        rsDesc.Desc_1_0.NumParameters = 1;
        rsDesc.Desc_1_0.NumStaticSamplers = 0;
        rsDesc.Desc_1_0.pStaticSamplers = 0;
        rsDesc.Desc_1_0.Flags =
                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        ID3DBlob *pSerializedRs = nullptr;
        ID3DBlob *errorMessage = nullptr;

        HRESULT serializerResult = D3D12SerializeVersionedRootSignature(&rsDesc, &pSerializedRs, &errorMessage);

        core::DebugTools::AssertAndThrow(serializerResult, static_cast<const char*>(errorMessage->GetBufferPointer()));

        ComPtr<ID3D12RootSignature> result;

        core::DebugTools::AssertAndThrow(device->CreateRootSignature(0,
                                                  pSerializedRs->GetBufferPointer(),
                                                  pSerializedRs->GetBufferSize(),
                                                  IID_PPV_ARGS(&result)),
                                                  "Failed to create a root signature!");

        return result;
    }

    RenderPipeline::RenderPipeline(RenderingContext ctx)
        : BaseObject(ctx)
    {

    }

    inline D3D12_INPUT_LAYOUT_DESC RenderPipeline::CreateInputLayout(){

        // todo: extend with color
        D3D12_INPUT_ELEMENT_DESC layout[] = {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
                  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };


        D3D12_INPUT_LAYOUT_DESC inputLayout = { };
        inputLayout.NumElements = 1;
        inputLayout.pInputElementDescs = layout;

        return inputLayout;
    }

    ComPtr<ID3D12PipelineState> RenderPipeline::CreatePipelineState(ComPtr<ID3D12Device> device, HShader vertexShader, HShader pixelShader){

        ComPtr<ID3D12PipelineState> result;

        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = { };

        desc.pRootSignature = m_RootSignature.Get();


        desc.PS.pShaderBytecode = pixelShader->GetShaderBlob()->GetBufferPointer();
        desc.PS.BytecodeLength = pixelShader->GetShaderBlob()->GetBufferSize();

        desc.VS.pShaderBytecode = vertexShader->GetShaderBlob()->GetBufferPointer();
        desc.VS.BytecodeLength = vertexShader->GetShaderBlob()->GetBufferSize();

        desc.BlendState.AlphaToCoverageEnable = false;
        desc.BlendState.IndependentBlendEnable = false;
        desc.BlendState.RenderTarget[0].BlendEnable = false;
        desc.BlendState.RenderTarget[0].LogicOpEnable = false;
		desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        desc.SampleMask = 0xFFFFFFFF;
        desc.SampleDesc = { 1, 0 };

        desc.RasterizerState = { };
        desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        desc.RasterizerState.FrontCounterClockwise = false;
        desc.RasterizerState.DepthClipEnable = true;
        desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        // todo: handle later
        desc.DepthStencilState = { };


        // todo: extend with color
        D3D12_INPUT_ELEMENT_DESC layout[] = {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
                  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(glm::vec3),
                  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        
        D3D12_INPUT_LAYOUT_DESC inputLayout = { };
        inputLayout.NumElements = 2;
        inputLayout.pInputElementDescs = layout;

        desc.InputLayout = inputLayout;

        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        desc.NumRenderTargets = 1;
        desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

        desc.NodeMask = 0;
        desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

        core::DebugTools::AssertAndThrow(device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&result)), "Failed to create a graphics pipeline state!");
        return result;
    }


    void RenderPipeline::Init(ComPtr<ID3D12Device> device, HShader vertexShader, HShader pixelShader) {

        m_Shaders[(int)ShaderType::PIXEL] = pixelShader;
        m_Shaders[(int)ShaderType::VERTEX] = vertexShader;

        m_RootSignature = CreateRootSignature(device);
        m_PipelineState = CreatePipelineState(device, vertexShader, pixelShader);

    }

    ComPtr<ID3D12RootSignature> RenderPipeline::GetRootSignature() {
        return m_RootSignature;
    }

    ComPtr<ID3D12PipelineState> RenderPipeline::GetPipelineState() {
        return m_PipelineState;
    }
}
