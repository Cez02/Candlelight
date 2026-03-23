#pragma once

namespace candle::core {

    template <class TContext>
    class BaseObject {
        TContext m_Context;

        BaseObject() = default;

    public:
        virtual ~BaseObject() = default;

        explicit BaseObject(TContext context) { m_Context = context; }

        virtual TContext GetContext() { return m_Context; }
    };
}
