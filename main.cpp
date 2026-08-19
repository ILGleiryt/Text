#if defined(__INTELLISENSE__)
#include <string>
#include <memory>
#else
import std;
#endif
#include "win_includes.hpp"
#include "win32_window.hpp"

int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    std::unique_ptr<Window> win = std::make_unique<Window>();

    if (!win->Create(400, 300, L"Privet"))
    {
        MessageBoxW(nullptr, L"Window doesnt created", L"Error", MB_OK);
        std::exit(-1);
    }

    win->RunMessageLoop();

    return 0;
}
