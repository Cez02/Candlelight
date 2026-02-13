#pragma once

#include <Windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

#include "candlelight_api.h"

#include "utils.h"
#include "renderer.h"

#if defined(CreateWindow)
#undef CreateWindow
#endif

namespace candle
{

/// @brief Core application class
class Application {
private:
    HWND m_hWnd;
    HINSTANCE m_hInst;
    std::shared_ptr<Renderer> m_Renderer;

    bool m_IsInitialized;

    void RegisterWindowClass(const wchar_t* windowClassName);

    HWND CreateWindow(const wchar_t* windowClassName
                     ,const wchar_t* windowTitle
                     ,uint32_t width
                     ,uint32_t height);

    static LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


public:

    void Init(HINSTANCE hInstance
             ,uint clientWidth
             ,uint clientHeight
             ,const wchar_t *windowTitle);

    bool IsInitialized();

    void Update();
    void Render();

    void Resize(uint width, uint height);

    ~Application();

    HWND getHWnd();
};

}