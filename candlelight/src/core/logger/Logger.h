#pragma once
#include <memory>
#include <string>

namespace spdlog {
    class logger;
}

namespace candle::core {
    class BaseContext;

    enum class LogType {
        Info,
        Warning,
        Error,
    };

    class Logger {
    protected:
        static std::unique_ptr<Logger> s_Instance;

        std::shared_ptr<spdlog::logger> m_Logger;

        void InitializeLocal(const std::string &loggerName, const std::string &fileName);

    public:
        static void Initialize(const std::string &loggerName, const std::string &fileName);
        static void Deinitialize();
        static void Log(LogType logType, const std::string &logMessage);
    };
}
