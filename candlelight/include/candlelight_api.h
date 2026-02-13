#pragma once

#include <candlelight_types.h>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl.h>

// ===================================
// Main engine API
// ===================================

namespace candle 
{

struct AppStartupSettings {
    uint m_ClientWidth = 300, m_ClientHeight = 300;
    bool m_UseWarp = false;
};

class Application;
typedef std::shared_ptr<Application> HApplication;

HApplication CreateApplication(HINSTANCE hInstance, AppStartupSettings settings);

void BeginLoop(HApplication application);

}


