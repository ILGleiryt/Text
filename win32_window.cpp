import std;
#include "win_includes.hpp"
#include "win32_window.hpp"

Window::Window() :
    main_handle(nullptr),
    is_created(false) {}

Window::~Window() noexcept
{
    if (GetHandle() && IsCreated())
    {
        if (!DestroyWindow(GetHandle()))
        {
            DWORD error = GetLastError();
            std::cerr << "Failed to destroy window. Error: " << error << "\n";
            PostQuitMessage(-1);
        }

        main_handle = nullptr;
        is_created = false;
    }
}

void Window::SetTitle(const std::wstring& title) const noexcept
{
    SetWindowTextW(GetHandle(), title.c_str());
}

void Window::Destroy() const noexcept
{
    PostMessageW(GetHandle(), WM_CLOSE, 0, 0);
}

void Window::Show() const noexcept
{
    ShowWindowAsync(GetHandle(), SW_SHOWNORMAL);
}

void Window::Minimize() const noexcept
{
    CloseWindow(GetHandle()); // work like a showwindow with sw_minimize??
}

void Window::Maximize() const noexcept
{
    ShowWindowAsync(GetHandle(), SW_SHOWMAXIMIZED);
}

LRESULT CALLBACK Window::WindowProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    UINT dpi;
    std::wstring dpi_str;
    WORD state = LOWORD(wparam);
    switch (message)
    {
    case WM_CREATE:
        dpi = GetDpiForWindow(handle);
        dpi_str = std::to_wstring(dpi);
        MessageBoxW(nullptr, dpi_str.c_str(), L"Window DPI", MB_OK);
        return 0;
    case WM_CLOSE:
        if (!DestroyWindow(handle))
        {
            DWORD error = GetLastError();
            std::cerr << "Failed to destroy window. Error: " << error << "\n";
            PostQuitMessage(-1);
        }
        return 0;
    case WM_ACTIVATE:
        if (state == WA_ACTIVE || state == WA_CLICKACTIVE)
        {
            OpenIcon(handle); // openicon work as showWindow with sw_restor option
        } // restore window size and pos 
        return 0;
    //case WM_ACTIVATEAPP: use if child window should be active
        //return 0; but main window is disabled
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    //case WM_CHAR:
        //return 0;
        //case WM_PAINT: // eat processor if uncommented
        //    return 0;
    case WM_KEYDOWN: // wparam for keycodes, lparam for info about scancode and info about counts 
        if (wparam == VK_ESCAPE)
        {
            PostMessageW(handle, WM_CLOSE, 0, 0);
            return 0;
        }
        return 0;
    //case WM_COMMAND: // for gui menu commands
        //return 0;
    //case WM_LBUTTONDOWN:
       // return 0;
    //case WM_LBUTTONUP:
    //case WM_SIZE:
    //case WM_MOUSEWHEEL:
    //case WM_MOVE:
        //break;
    //case WM_WINDOWPOSCHANGED: return *WINDOWPOS structure with new window dimensions
        //return 0;
    default:
        return DefWindowProcW(handle, message, wparam, lparam);
    }
    return 0;
}

int Window::RunMessageLoop() const noexcept
{
    MSG message {};
    while (GetMessageW(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}

bool Window::Create(int width, int height, const std::wstring& title)
{
    /*work with windows vista and above*/ SetProcessDPIAware(); // use for correctly work with not standard dpi (dpi > 96)
    // SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE) work from windows version 8.1
    // SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2) only from windows 10

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

    main_handle = CreateWindowExW(WS_EX_TOPMOST, MainWindow.lpszClassName, title.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        width, height, nullptr, nullptr, MainWindow.hInstance, nullptr);

    if (!main_handle)
        return false;

    is_created = true;
    ShowWindowAsync(GetHandle(), SW_SHOWDEFAULT);
    UpdateWindow(GetHandle());

    return true;
}
