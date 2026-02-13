#include "debug.h"

#include <helpers.h>
#include <vector>

namespace candle {

    Debug *Debug::s_Instance = nullptr;

    void Debug::Init() {
        if(!m_Instance.Get()) {
            ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&m_Instance)));
        }
    }

    void Debug::Enable() {
        Init();

        s_Instance = this;

        m_Instance->EnableDebugLayer();
    }

    void Debug::SetDevice(ComPtr<ID3D12Device> device) {
        m_Device = device;
    }

    Debug *Debug::GetInstance() {
        return s_Instance;
    }

    void Debug::DumpLogs() {
        if (s_Instance != nullptr) {
            s_Instance->DumpLogsLocal();
        }
    }

    void Debug::DumpLogsLocal() {
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

                std::cout << msg->pDescription << std::endl;
            }

            infoQueue->ClearStoredMessages();
        }
    }

    Debug::~Debug() {
        s_Instance = nullptr;
    }
}
