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
#include <stdio.h>
#include <string>

const std::wstring name = L"Bob";

static LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message,WPARAM wparam, LPARAM lparam)
{
    UINT a;
    std::wstring bc;
    switch (message)
    {
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE)
        {
            PostMessageW(window_handle, WM_CLOSE, 0, 0);
            return 0;
        }
        return 0;
    case WM_CREATE:
         a = GetDpiForWindow(window_handle);
         bc = std::to_wstring(a);
         MessageBoxW(nullptr, bc.c_str(), L"DPI", 0);
         return 0;
    case WM_CLOSE:
        DestroyWindow(window_handle);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window_handle, message, wparam, lparam);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
     _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{

    WNDCLASSEXW winclass = { 0 };

    winclass.cbSize = sizeof(WNDCLASSEXW),
    winclass.style = CS_OWNDC;
    winclass.lpfnWndProc = WindowProcedure; //DefWindowProcW
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = GetModuleHandleW(nullptr);
    winclass.hIcon = nullptr;
    winclass.hCursor = nullptr;
    winclass.hbrBackground = nullptr;
    winclass.lpszMenuName = nullptr;
    winclass.lpszClassName = L"WindowName";
    winclass.hIconSm = nullptr;

    if (!RegisterClassExW(&winclass))
        return 1;

    HWND window_handle = CreateWindowExW(0, winclass.lpszClassName, L"Window",
        WS_OVERLAPPEDWINDOW/*(window_style)*/, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, nullptr, nullptr, hInstance, nullptr);

    if (!window_handle) {
        return 1;
    }

    MessageBoxExW(nullptr, L"Write some text", L"Caption", MB_OK, 0);

    ShowWindow(window_handle, SW_SHOW);
    UpdateWindow(window_handle);

    MSG message {};
    while (GetMessageW(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}
