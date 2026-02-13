#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

#include <wrl.h>
using namespace Microsoft::WRL;

#include <memory>

#include <candlelight_api.h>
#include <iostream>

void ParseCommandLineArguments(candle::AppStartupSettings& settings)
{
    int argc;
    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

    for (size_t i = 0; i < argc; ++i)
    {
        if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0)
        {
            settings.m_ClientWidth = ::wcstol(argv[++i], nullptr, 10);
        }
        if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0)
        {
            settings.m_ClientHeight = ::wcstol(argv[++i], nullptr, 10);
        }
        if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0)
        {
            settings.m_UseWarp = true;
        }
    }

    // Free memory allocated by CommandLineToArgvW
    ::LocalFree(argv);
}

void AttachDebugConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    std::cout.clear();
    std::cerr.clear();
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    AttachDebugConsole();

    // Settings
    std::cout << "Reading application settings." << std::endl;;


    candle::AppStartupSettings settings;

    ParseCommandLineArguments(settings);


    // Application init

    std::cout << "Creating application." << std::endl;;

    candle::HApplication application = candle::CreateApplication(hInstance, settings);

    std::cout << "Starting loop." << std::endl;;


    candle::BeginLoop(application);

}