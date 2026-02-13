#ifndef CANDLELIGHT_DEBUG_H
#define CANDLELIGHT_DEBUG_H

#include <wrl.h>
using namespace Microsoft::WRL;

#include <memory>

#include <d3d12sdklayers.h>

namespace candle {

class Debug {
private:
    ComPtr<ID3D12Debug> m_Instance;
    ComPtr<ID3D12Device> m_Device;

    static Debug *s_Instance;

public:
    void Init();
    void Enable();
    void SetDevice(ComPtr<ID3D12Device> device);

    static Debug *GetInstance();
    static void DumpLogs();

    void DumpLogsLocal();

    ~Debug();
};

typedef std::shared_ptr<Debug> HDebug;


}
#endif //CANDLELIGHT_DEBUG_H
