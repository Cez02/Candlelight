#pragma once
#include "Basic.h"
#include "GUID.h"

namespace candle::core {
    class BaseRawObject {
    private:
        CandleGUID m_GUID;

    public:
        virtual ~BaseRawObject() = default;

        BaseRawObject()
            : m_GUID(CreateGUID()){ }

        CandleGUID GetGUID() const { return m_GUID; }
    };
}
