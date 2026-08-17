#include "win_includes.hpp"
#include "win32_window.hpp"
#include "IEventHandler.h"
#include <string>

//RECT Window::GetClientRect()
//{
//    RECT a {};
//    ::GetClientRect(main_handle, &a);
//    client_width = a.right - a.left;
//    client_height = a.bottom - a.top;
//}

void Window::SetTitle(const std::wstring& title) const
{
    SetWindowTextW(main_handle, title.c_str());
}

//void Window::SetEventHandler(IEventHandler* handler)
//{
//}

LRESULT CALLBACK Window::WindowProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    UINT a;
    std::wstring bc;
    switch (message)
    {
    case WM_CREATE:
        a = GetDpiForWindow(handle);
        bc = std::to_wstring(a);
        MessageBoxW(nullptr, bc.c_str(), L"DPI", 0);
        return 0;
    case WM_CLOSE:
        DestroyWindow(handle);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CHAR:
        return 0;
        //case WM_PAINT:
        //    return 0;
    case WM_KEYDOWN: // wparam for keycodes, lparam for info about scancode and info about counts 
        if (wparam == VK_ESCAPE)
        {
            PostMessageW(handle, WM_CLOSE, 0, 0);
            return 0;
        }
        return 0;
    case WM_COMMAND: // for gui menu commands
        return 0;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_SIZE:
    case WM_MOUSEWHEEL:
        break;
    default:
        return DefWindowProcW(handle, message, wparam, lparam);
    }
    return 0;
}

Window::Window() :
    main_handle(nullptr),
    //event_handler(nullptr),
    is_created(false)
{}

Window::~Window() noexcept
{
    if (main_handle && is_created)
    {
        DestroyWindow(main_handle);
        main_handle = nullptr;
        is_created = false;
    }
}

int Window::RunMessageLoop()
{
    MSG message {};
    while (GetMessageW(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}

void Window::Show() const noexcept
{
    ShowWindowAsync(main_handle, SW_SHOWDEFAULT);
}

bool Window::Create(int width, int height, const std::wstring& title)
{
    WNDCLASSEXW MainWindow = { 0 };

    MainWindow.cbSize = sizeof(WNDCLASSEXW),
    MainWindow.style = CS_OWNDC;
    MainWindow.lpfnWndProc = WindowProcedure; //DefWindowProcW
    MainWindow.cbClsExtra = 0;
    MainWindow.cbWndExtra = 0;
    MainWindow.hInstance = GetModuleHandleW(nullptr);
    MainWindow.hIcon = nullptr;
    MainWindow.hCursor = nullptr;
    MainWindow.hbrBackground = nullptr;
    MainWindow.lpszMenuName = nullptr;
    MainWindow.lpszClassName = L"MainWindow";
    MainWindow.hIconSm = nullptr;

    if (!RegisterClassExW(&MainWindow))
        return false;

    main_handle = CreateWindowExW(0, MainWindow.lpszClassName, title.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        width, height, nullptr, nullptr, MainWindow.hInstance, nullptr);
    Show();
    UpdateWindow(main_handle);
    is_created = true;
    return true;
}



void Window::Hide() const noexcept
{
    CloseWindow(main_handle);
}
