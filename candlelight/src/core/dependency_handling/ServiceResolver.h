#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <any>
#include <format>

#include "Logger.h"

namespace candle::core {
    class ServiceResolver {
        std::unordered_map<std::type_index, std::any> m_RegisteredServices;

    public:
        template<typename T>
        void Register(std::shared_ptr<T> service)
        {
            m_RegisteredServices[typeid(T)] = service;
            Logger::Log(LogType::Info, std::format("Registering new service with type hash {}", typeid(T).hash_code()));
        }

        template<typename T>
        std::shared_ptr<T> Get()
        {
            auto it = m_RegisteredServices.find(std::type_index(typeid(T)));
            if (it != m_RegisteredServices.end()) {
                return std::any_cast<std::shared_ptr<T>>(it->second);
            }
            return nullptr;
        }
    };

    typedef std::shared_ptr<ServiceResolver> HServiceResolver;
}
