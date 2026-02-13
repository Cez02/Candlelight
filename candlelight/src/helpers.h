#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // For HRESULT
#include <exception>
#include <iostream>
#include <string>
#include <directx/d3d12.h>

#include <debug.h>

#define KBsToBytes(kbs) ((kbs) * 1024)

namespace candle
{

// From DXSampleHelper.h 
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
inline void ThrowIfFailed(HRESULT hr)
{
    Debug::DumpLogs();

    if (FAILED(hr))
    {
        std::cerr << "FAILED WITH " << ((unsigned int)hr) << std::endl;
        throw std::exception();
    }
}

inline void ShoutAndThrowIfFailed(HRESULT hr, ID3DBlob *message){

    if(message != nullptr){
        std::cerr << (const char*)message->GetBufferPointer() << std::endl;
    }

    ThrowIfFailed(hr);
}

}