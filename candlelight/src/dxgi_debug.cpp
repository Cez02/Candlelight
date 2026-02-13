#include "dxgi_debug.h"

#include <helpers.h>

namespace candle {

    void DXGIDebug::Init() {
        if(!m_Instance.Get()) {
            ThrowIfFailed(DXGIGetDebugInterface(IID_PPV_ARGS(&m_Instance)));
        }
    }

    void DXGIDebug::Enable() {
        Init();

        m_Instance->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
    }

    DXGIDebug::~DXGIDebug() {


    }


}