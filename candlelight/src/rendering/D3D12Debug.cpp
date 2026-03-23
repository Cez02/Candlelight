#include "D3D12Debug.h"

#include <vector>

#include "DebugTools.h"
#include "Logger.h"

using namespace candle::core;

namespace candle::rendering {

    HD3D12Debug D3D12Debug::s_Instance = nullptr;

    void D3D12Debug::Init() {
        if(!m_ID3D12Instance.Get()) {
            DebugTools::AssertAndThrow(D3D12GetDebugInterface(IID_PPV_ARGS(&m_ID3D12Instance)), "Failed to get D3D12 debug interface!");
        }

        if (!m_IDXGIInstance.Get()) {
            DebugTools::AssertAndThrow(DXGIGetDebugInterface(IID_PPV_ARGS(&m_IDXGIInstance)), "Failed to get DXGI instance!");
        }
    }

    void D3D12Debug::Enable() {
        Init();

        m_ID3D12Instance->EnableDebugLayer();
        m_IDXGIInstance->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
    }

    void D3D12Debug::SetDevice(ComPtr<ID3D12Device> device) {
        m_Device = device;
    }

    HD3D12Debug D3D12Debug::GetInstance() {
        return s_Instance;
    }

    void D3D12Debug::DumpLogs() {
        if (s_Instance != nullptr) {
            s_Instance->DumpLogsLocal();
        }
    }

    HD3D12Debug D3D12Debug::CreateInstance() {
        if (s_Instance != nullptr) {
            core::Logger::Log(core::LogType::Warning, "Cannot create a second instance of debug!");
            return s_Instance;
        }

        s_Instance = std::make_shared<D3D12Debug>();
        return s_Instance;
    }

    void D3D12Debug::DumpLogsLocal() {
        if (m_Device.Get() == nullptr) {
            return;
        }

        ComPtr<ID3D12InfoQueue> infoQueue;
        if (SUCCEEDED(m_Device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
        {
            const UINT64 count = infoQueue->GetNumStoredMessages();

            for (UINT64 i = 0; i < count; i++)
            {
                SIZE_T len = 0;
                infoQueue->GetMessage(i, nullptr, &len);

                std::vector<char> bytes(len);
                auto* msg = reinterpret_cast<D3D12_MESSAGE*>(bytes.data());

                infoQueue->GetMessage(i, msg, &len);

                Logger::Log(LogType::Info, msg->pDescription);
            }

            infoQueue->ClearStoredMessages();
        }
    }

    void D3D12Debug::Deinit() {
        s_Instance = nullptr;
    }
}
