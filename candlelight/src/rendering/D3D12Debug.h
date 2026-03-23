#ifndef CANDLELIGHT_DEBUG_H
#define CANDLELIGHT_DEBUG_H

#include <wrl.h>
using namespace Microsoft::WRL;

#include <memory>

#include <d3d12sdklayers.h>
#include <dxgidebug.h>

namespace candle::rendering {

    class D3D12Debug;
    typedef std::shared_ptr<D3D12Debug> HD3D12Debug;

    class D3D12Debug {
    private:
        ComPtr<ID3D12Debug> m_ID3D12Instance;
        ComPtr<IDXGIDebug> m_IDXGIInstance;
        ComPtr<ID3D12Device> m_Device;

        static HD3D12Debug s_Instance;

    public:
        void Init();
        void Enable();
        void SetDevice(ComPtr<ID3D12Device> device);

        static HD3D12Debug GetInstance();
        static void DumpLogs();

        static HD3D12Debug CreateInstance();

        void DumpLogsLocal();

        static void Deinit();
    };

}
#endif //CANDLELIGHT_DEBUG_H
