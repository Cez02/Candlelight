#pragma once
#include "BaseRawObject.h"

namespace candle::core {

    template <class TContext>
    class BaseObject : BaseRawObject {
        TContext m_Context;

        BaseObject() = default;

    public:
        ~BaseObject() override = default;

        explicit BaseObject(TContext context) { m_Context = context; }

        virtual TContext GetContext() { return m_Context; }
    };
}
