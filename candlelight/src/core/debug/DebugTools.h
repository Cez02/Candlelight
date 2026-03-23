#pragma once

#include <intsafe.h>
#include <string>

namespace candle::core {

    class DebugTools {
    public:
        static void AssertAndLog(bool condition, const std::string &errorMessage, void(*assertFailedCallback)() = nullptr);
        static void AssertAndThrow(bool condition, const std::string &errorMessage, void(*assertFailedCallback)() = nullptr);

        static void AssertAndLog(HRESULT result, const std::string &errorMessage, void(*assertFailedCallback)() = nullptr);
        static void AssertAndThrow(HRESULT result, const std::string &errorMessage, void(*assertFailedCallback)() = nullptr);

    };

}
