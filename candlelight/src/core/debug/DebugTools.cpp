#include "DebugTools.h"

#include <stdexcept>

#include "Logger.h"

namespace candle::core {
    void DebugTools::AssertAndLog(bool condition, const std::string &errorMessage, void(*assertFailedCallback)) {
        if (condition)
            return;

        Logger::Log(LogType::Error, errorMessage);

        if (assertFailedCallback != nullptr) {
            assertFailedCallback();
        }
    }

    void DebugTools::AssertAndThrow(bool condition, const std::string &errorMessage, void(*assertFailedCallback)) {
        if (condition)
            return;

        Logger::Log(LogType::Error, errorMessage);
        if (assertFailedCallback != nullptr) {
            assertFailedCallback();
        }

        throw std::runtime_error(errorMessage);
    }

    void DebugTools::AssertAndLog(HRESULT result, const std::string &errorMessage, void *assertFailedCallback) {
        if (!result)
            return;

        Logger::Log(LogType::Error, errorMessage);

        if (assertFailedCallback != nullptr) {
            assertFailedCallback();
        }
    }

    void DebugTools::AssertAndThrow(HRESULT result, const std::string &errorMessage, void *assertFailedCallback) {
        if (!result)
            return;

        Logger::Log(LogType::Error, errorMessage);
        if (assertFailedCallback != nullptr) {
            assertFailedCallback();
        }

        throw std::runtime_error(errorMessage);
    }
}
