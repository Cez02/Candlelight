
#include <candlelight_api.h>
#include <candlelight_types.h>

#include <chrono>
#include "application.h"

#include "utils.h"

using namespace candle;

using namespace Microsoft::WRL;

void candle::Application::RegisterWindowClass(const wchar_t* windowClassName)
{
    std::cout << "Registering window class" << std::endl;

    // Register a window class for creating our render window with.
    WNDCLASSEXW windowClass = {};

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = &Application::WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = m_hInst;
    windowClass.hIcon = ::LoadIcon(m_hInst, NULL);
    windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = windowClassName;
    windowClass.hIconSm = ::LoadIcon(m_hInst, NULL);

    static ATOM atom = ::RegisterClassExW(&windowClass);
    assert(atom > 0);
}

HWND candle::Application::CreateWindow(const wchar_t* windowClassName
                                      ,const wchar_t* windowTitle
                                      ,uint32_t width
                                      ,uint32_t height)
{
    int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

    RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    // Center the window within the screen. Clamp to 0, 0 for the top-left corner.
    int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
    int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

    HWND hWnd = ::CreateWindowExW(
        0,
        windowClassName,
        windowTitle,
        WS_OVERLAPPEDWINDOW,
        windowX,
        windowY,
        windowWidth,
        windowHeight,
        NULL,
        NULL,
        m_hInst,
        this
    );

    assert(hWnd && "Failed to create window");

    return hWnd;
}


LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Application *application;

    if (message == WM_NCCREATE)
    {
        application = static_cast<Application*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
    {
        application = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }


    if ( application != nullptr && application->IsInitialized() )
    {
        switch (message)
        {
        case WM_PAINT:
            application->Update();
            application->Render();
            break;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

            switch (wParam)
            {
            // case 'V':
            //     g_VSync = !g_VSync;
            //     break;
            case VK_ESCAPE:
                ::PostQuitMessage(0);
                break;
            // case VK_RETURN:
            //     if ( alt )
            //     {
            // case VK_F11:
            //     SetFullscreen(!g_Fullscreen);
            //     }
            //     break;
            }
        }
        break;
        // The default window procedure will play a system notification sound 
        // when pressing the Alt+Enter keyboard combination if this message is 
        // not handled.
        case WM_SYSCHAR:
        break;
        case WM_SIZE:
        {
            RECT clientRect = {};
            ::GetClientRect(application->getHWnd(), &clientRect);

            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;

            application->Resize(width, height);
        }
        break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        }
    }


    return ::DefWindowProcW(hwnd, message, wParam, lParam);
}

void candle::Application::Init(HINSTANCE hInstance, uint clientWidth, uint clientHeight, const wchar_t *windowTitle){

    // Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
    // Using this awareness context allows the client area of the window 
    // to achieve 100% scaling while still allowing non-client window content to 
    // be rendered in a DPI sensitive fashion.
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    std::cout << "Creating window name" << std::endl;

    // Window class name. Used for registering / creating the window.
    const wchar_t* windowClassName = L"DX12WindowClass";

    std::cout << "Creating window name" << std::endl;

    m_hInst = hInstance;

    std::cout << "Registering a window..." << std::endl;

    RegisterWindowClass(windowClassName);

    std::cout << "Creating a window..." << std::endl;

    m_hWnd = CreateWindow(windowClassName, windowTitle, clientWidth, clientHeight);

    std::cout << "Initializing renderer..." << std::endl;

    m_Renderer = std::make_shared<Renderer>();
    m_Renderer->Init(m_hWnd, clientWidth, clientHeight, false);

    m_IsInitialized = true;
}

bool candle::Application::IsInitialized()
{
    return m_IsInitialized;
}

void candle::Application::Update()
{
    static uint64_t frameCounter = 0;
    static double elapsedSeconds = 0.0;
    static std::chrono::high_resolution_clock clock;
    static auto t0 = clock.now();

    frameCounter++;
    auto t1 = clock.now();
    auto deltaTime = t1 - t0;
    t0 = t1;

    elapsedSeconds += deltaTime.count() * 1e-9;
    if (elapsedSeconds > 1.0)
    {
        char buffer[500];
        auto fps = frameCounter / elapsedSeconds;
        sprintf_s(buffer, 500, "FPS: %f\n", fps);
        OutputDebugString(buffer);

        frameCounter = 0;
        elapsedSeconds = 0.0;
    }
}

void candle::Application::Render()
{
    m_Renderer->Render();
}

void candle::Application::Resize(uint width, uint height)
{
}

candle::Application::~Application(){

    // TODO

}

HWND candle::Application::getHWnd()
{
    return m_hWnd;
}

HApplication candle::CreateApplication(HINSTANCE hInstance, AppStartupSettings settings){

    HApplication application = std::make_shared<Application>();

    application->Init(hInstance, settings.m_ClientWidth, settings.m_ClientWidth, L"Example title");

    return application;
}

void candle::BeginLoop(HApplication application){

    ::ShowWindow(application->getHWnd(), SW_SHOW);

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        std::cout << "Loop" << std::endl;
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else 
        {
            application->Update();
            application->Render();
        }
    }
}

