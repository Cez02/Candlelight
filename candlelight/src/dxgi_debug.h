#ifndef CANDLELIGHT_DEBUG_H
#define CANDLELIGHT_DEBUG_H

#include <wrl.h>
using namespace Microsoft::WRL;

#include <memory>

#include <dxgidebug.h>

namespace candle {

class DXGIDebug {
private:
    ComPtr<IDXGIDebug> m_Instance;

public:

    void Init();
    void Enable();

    ~DXGIDebug();
};

typedef std::shared_ptr<Debug> HDebug;

}
#endif //CANDLELIGHT_DEBUG_H
