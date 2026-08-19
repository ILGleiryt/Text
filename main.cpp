#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#if defined(__INTELLISENSE__)
#include <string>
#include <memory> // HACK FOR INTELLISENSE WORKING FOR MODULES
#else
import std;
#endif
import Module;

int CALLBACK wWinMain( 
    [[maybe_unused]] HINSTANCE hInstance, 
    [[maybe_unused]] HINSTANCE hPrevInstance,
    [[maybe_unused]] PWSTR pCmdLine,
    [[maybe_unused]] int nCmdShow)
{
    auto win = std::make_unique<Window>();

    if (!win->Create(400, 300, L"Privet"))
    {
        MessageBoxW(nullptr, L"Register Window class or create window return error",
            L"Window creation error", MB_ICONWARNING|MB_OK);
        std::exit(-1);
    }

    
    return win->RunMessageLoop();
}
