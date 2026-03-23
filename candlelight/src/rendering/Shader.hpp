#pragma once

#include <memory>
#include <filesystem>
#include <vector>

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace candle::rendering
{

    enum class ShaderType {
        VERTEX,
        PIXEL,
        COUNT
    };

    class Shader {
    private:
        ComPtr<IDxcBlob> m_ShaderBlob;

    public:
        void Init(std::string shaderSourceFilePath, const ShaderType shaderType);

        ComPtr<IDxcBlob> GetShaderBlob();

        void Release();
        ~Shader();
    };

    typedef std::shared_ptr<Shader> HShader;

}
