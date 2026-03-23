#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <any>

namespace candle::core {
    class ServiceResolver {
        std::unordered_map<std::type_index, std::any> m_RegisteredServices;

    public:
        template<typename T>
        void Register(std::shared_ptr<T> service);

        template<typename T>
        std::shared_ptr<T> Get();
    };

    typedef std::shared_ptr<ServiceResolver> HServiceResolver;
}