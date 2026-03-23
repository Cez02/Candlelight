#include "ServiceResolver.h"

#include <format>
#include <memory>

#include "Logger.h"

namespace candle::core {
    template<typename T>
    void ServiceResolver::Register(std::shared_ptr<T> service)
    {
        m_RegisteredServices[typeid(T)] = service;
        Logger::Log(LogType::Info, std::format("Registering new service with type hash {}", typeid(T).hash_code()));
    }

    template<typename T>
    std::shared_ptr<T> ServiceResolver::Get()
    {
        auto it = m_RegisteredServices.find(std::type_index(typeid(T)));
        if (it != m_RegisteredServices.end()) {
            return std::any_cast<std::shared_ptr<T>>(it->second);
        }
        return nullptr;
    }
}
