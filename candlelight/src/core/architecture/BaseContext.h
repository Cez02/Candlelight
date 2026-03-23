#pragma once

#include <stdexcept>

#include "ServiceResolver.h"

namespace candle::core {
    class BaseContext {
    protected:
        bool m_Locked = false;
        HServiceResolver m_Resolver;

        HWND m_hWnd = nullptr;
        HINSTANCE m_hInst = nullptr;

    public:
        void LockContext() { m_Locked = true; }
        void UnlockContext() { m_Locked = false; }

        void InitializeResolver() {
            m_Resolver = std::make_shared<ServiceResolver>();
        }

        HServiceResolver GetResolver() const { return m_Resolver; }

        HWND GetHWND() const { return m_hWnd; }
        void SetHWND(const HWND hWnd) { if (!m_Locked) m_hWnd = hWnd; else throw std::runtime_error(__FUNCSIG__); }

        HINSTANCE GetHINSTANCE() const { return m_hInst; }
        void SetHINSTANCE(HINSTANCE hInstance) { if (!m_Locked) m_hInst = hInstance; else throw std::runtime_error(__FUNCSIG__); }

    };
}
