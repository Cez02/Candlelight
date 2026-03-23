#pragma once

namespace candle::core::memory {

    template<typename T> constexpr
    T const& Kbs2Bytes(T const&v){ return 1024 * v; }

}