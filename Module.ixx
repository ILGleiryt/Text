module;
#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOMINMAX
#define NOGDI
#define NOSOUND
#define NOSERVICE
#define NONLS
#define NOCOMM
#define STRICT
#include <Windows.h>
#include <windowsx.h>
#include <cassert>

export module Module;

import std;
constexpr int DEFAULT_DPI = USER_DEFAULT_SCREEN_DPI;

export class Window
{
public:
    [[nodiscard]] Window() {
        main_handle = nullptr, is_created = false;
    }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window() noexcept 
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

    int RunMessageLoop() const noexcept 
    {
        MSG message { 0 };
        while (GetMessageW(&message, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
        return static_cast<int>(message.wParam);
    }

    [[nodiscard]] bool Create(int width, int height, const std::wstring& title)
    {
        assert(width > 0 && "Use positive or upper 0 width");
        assert(height > 0 && "Use positive or upper 0 height");
        /*work with windows vista and above*/ //SetProcessDPIAware(); // use for correctly work with not standard dpi (dpi > 96)
        // SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE) work from windows version 8.1
        // SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2) only from windows 10

        WNDCLASSEXW MainWindow = { 0 };

        MainWindow.cbSize = sizeof(WNDCLASSEXW);
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

        assert(MainWindow.cbSize == sizeof(WNDCLASSEXW));
        assert(MainWindow.hInstance != nullptr);
        assert(MainWindow.lpfnWndProc != nullptr);
        assert(MainWindow.lpszClassName != nullptr);

        if (!RegisterClassExW(&MainWindow))
            return false;

        main_handle = CreateWindowExW(WS_EX_TOPMOST, MainWindow.lpszClassName, title.c_str(),
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            width, height, nullptr, nullptr, MainWindow.hInstance, nullptr);

        assert(main_handle != nullptr);

        if (!main_handle)
            return false;

        is_created = true;
        ShowWindowAsync(GetHandle(), SW_SHOWDEFAULT);
        UpdateWindow(GetHandle());

        return true;
    }

    void SetTitle(const std::wstring& title) const noexcept
    {
        assert(title != L"" && "Window must be named");
        if (title == L"") return;
        SetWindowTextW(GetHandle(), title.c_str());
    }

    void Destroy() const noexcept
    {
        PostMessageW(GetHandle(), WM_CLOSE, 0, 0);
    }

    void Show() const noexcept
    {
        ShowWindowAsync(GetHandle(), SW_SHOWNORMAL);
    }

    void Minimize()  const noexcept
    {
        CloseWindow(GetHandle()); // work like a showwindow with sw_minimize??
    }
    void Maximize() const noexcept
    {
        ShowWindowAsync(GetHandle(), SW_SHOWMAXIMIZED);
    }
    [[nodiscard]] HWND GetHandle() const noexcept { return this->main_handle; }
    [[nodiscard]] bool IsCreated() const noexcept { return this->is_created; }
    // when first wm_create appears i use dpi_check i store pdi value in storage
    // and use it with every wm_pain for correct size
    // maybe i need look to wm_dpichange?
    //int GetWidth() const;
    //int GetHeight() const;
    //RECT GetClientRect();
    //resize?, work with dpi?,

private:
    static LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
    {
        UINT dpi = 0;
        //int scale_dpi = dpi / DEFAULT_DPI ;
        std::wstring dpi_str;
        WORD state = LOWORD(wparam);
        switch (message)
        {
        case WM_DPICHANGED: 
        {
            dpi = HIWORD(wparam);
            const RECT* rc = (RECT*)lparam;
            SetWindowPos(handle,
                NULL,
                rc->left,
                rc->top,
                rc->right - rc->left,
                rc->bottom - rc->top,
                SWP_NOZORDER | SWP_NOACTIVATE);
            return 0;
        }

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

    int dpi = DEFAULT_DPI;
    //getClientWidth, getClientHeight
    //getWindowWidth, getWindowHeight
    bool is_created   { false };
    bool is_maximized { false };
    bool is_collapsed { false };
    bool fullscreen   { false }; // store data and write it to storage file
    int client_width  {};
    int client_height {};

    HINSTANCE instance_handle {};
    HWND main_handle {};
};
