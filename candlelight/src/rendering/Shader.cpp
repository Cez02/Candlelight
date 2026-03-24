#include "Shader.hpp"
#include <fstream>
#include <string>
#include <filesystem>

#include <d3dcompiler.h>
#include <dxcapi.h>

#include "Logger.h"

namespace candle::rendering
{
    const wchar_t* ChooseShaderTarget(const ShaderType shaderType){

        switch(shaderType){
            case ShaderType::VERTEX:
                return L"vs_6_0";
            case ShaderType::PIXEL:
                return L"ps_6_0";
        }

        core::Logger::Log(core::LogType::Error, std::format("Unrecognized shader target type {}", (int)shaderType));

        return L"vs_6_0";
    }


    void Shader::Init(std::string shaderSourceFilePath, const ShaderType shaderType){

        core::Logger::Log(core::LogType::Info, std::format("Compiling shader {}", shaderSourceFilePath));

        // prepare compilation tools
        ComPtr<IDxcUtils> utils;
        ComPtr<IDxcCompiler3> compiler;
        ComPtr<IDxcIncludeHandler> includeHandler;

        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
        DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

        utils->CreateDefaultIncludeHandler(&includeHandler);


        // Read shader source
        std::uintmax_t filesize = std::filesystem::file_size(std::filesystem::path(shaderSourceFilePath));
        std::ifstream fin(shaderSourceFilePath, std::ios::in );
        std::string shaderSource;

        shaderSource.resize(filesize);
        fin.read(shaderSource.data(), filesize);

        fin.close();

        // Compile

        DxcBuffer source = {};
        source.Ptr = shaderSource.data();
        source.Size = shaderSource.size();
        source.Encoding = DXC_CP_UTF8;

        std::vector<LPCWSTR> args = {
            L"-E", L"main",        // entry point
            L"-T", ChooseShaderTarget(shaderType),        // shader model,
            L"-Wno-null-character"
    #ifdef _DEBUG
            , L"-Zi",                  // debug info optional
            L"-Qembed_debug"
    #endif
        };

        ComPtr<IDxcResult> result;

        compiler->Compile(
            &source,
            args.data(),
            args.size(),
            includeHandler.Get(),
            IID_PPV_ARGS(&result)
        );

        ComPtr<IDxcBlobUtf8> errors;
        result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);

        if (errors && errors->GetStringLength() > 0) {
            core::Logger::Log(core::LogType::Error, std::format("Loaded shader {}", errors->GetStringPointer()));
        }

        result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&m_ShaderBlob), nullptr);

        core::Logger::Log(core::LogType::Info, std::format("Loaded shader {}", shaderSourceFilePath));
    }

    void Shader::Release(){
        if (m_ShaderBlob.Get() != nullptr) {
            m_ShaderBlob->Release();
        }
    }

    Shader::~Shader(){
        Release();
    }

    ComPtr<IDxcBlob> Shader::GetShaderBlob() {
        return m_ShaderBlob;
    }

}