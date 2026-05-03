#pragma once
#include <iomanip>
#include <random>
#include <sstream>

namespace candle::core {

    typedef long long CandleGUID;

    // Should probably replace this with something better in the future
    inline CandleGUID CreateGUID() {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0,255);

        CandleGUID result = 0;
        for (int i = 0; i < sizeof(CandleGUID); ++i) {
            result += dist6(rng);
            result <<= 8;
        }

        return result;
    }

    inline std::string to_string(CandleGUID guid) {
        std::stringstream stream;
        stream << "0x"
               << std::setfill ('0') << std::setw(sizeof(CandleGUID)*2)
               << std::hex << guid;
        return stream.str();
    }

    inline std::wstring to_wstring(CandleGUID guid) {
        std::wstringstream stream;
        stream << "0x"
               << std::setfill (L'0') << std::setw(sizeof(CandleGUID)*2)
               << std::hex << guid;
        return stream.str();
    }

}
