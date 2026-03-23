#pragma once

#include <Windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

#include "../rendering/Renderer.h"

#include "WindowsUtils.h"

#if defined(CreateWindow)
#undef CreateWindow
#endif

namespace candle
{

    /// @brief Core application class
    class Application {
    private:
        // Context
        core::BaseContext m_Context = {};

        // Modules
        rendering::HRenderer m_Renderer;

        // Data
        bool m_IsInitialized = false;

        // Methods
        void RegisterWindowClass(const wchar_t* windowClassName) const;
        HWND CreateWindow(const wchar_t* windowClassName
                         ,const wchar_t* windowTitle
                         ,uint32_t width
                         ,uint32_t height);
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    public:
        void Init(HINSTANCE hInstance
                 ,uint clientWidth
                 ,uint clientHeight
                 ,const wchar_t *windowTitle);

        bool IsInitialized();

        static void Update();
        void Render() const;

        void Resize(uint width, uint height);

        ~Application();

        core::BaseContext GetContext() const { return m_Context; }
    };

}