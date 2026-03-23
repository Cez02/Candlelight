#include "Logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace candle::core {
    void Logger::Initialize(const std::string &loggerName, const std::string &fileName) {
        if (s_Instance != nullptr) {
            spdlog::warn("Logger is already initialized! Ignoring.");
            return;
        }

        s_Instance = std::make_unique<Logger>();
        s_Instance->InitializeLocal(loggerName, fileName);
    }

    void Logger::Deinitialize() {
        s_Instance = nullptr;
    }

    void Logger::Log(LogType logType, const std::string &logMessage) {
        switch (logType) {
            case LogType::Info:
                s_Instance->m_Logger->info(logMessage);
                return;
            case LogType::Warning:
                s_Instance->m_Logger->warn(logMessage);
                return;
            case LogType::Error:
                s_Instance->m_Logger->error(logMessage);
        }
    }

    std::unique_ptr<Logger> Logger::s_Instance = nullptr;

    void Logger::InitializeLocal(const std::string &loggerName, const std::string &fileName) {
        m_Logger = spdlog::basic_logger_mt(loggerName, fileName);
    }
}
