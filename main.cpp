//#include "win_includes.hpp"
#include "win_includes.hpp"
#include <string>
#include "win32_window.hpp"
int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    Window* win = new Window();
    win->Create(600, 500, L"HelloWorld");
    win->RunMessageLoop();
    return 0;
}
//
//struct WindowInfo
//{
//
//};
//WindowInfo* info = new WindowInfo;
//
//static LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message,WPARAM wparam, LPARAM lparam)
//{
//    UINT a;
//    std::wstring bc;
//    switch (message)
//    {
//    case WM_CREATE:
//        a = GetDpiForWindow(window_handle);
//        bc = std::to_wstring(a);
//        MessageBoxW(nullptr, bc.c_str(), L"DPI", 0);
//        return 0;
//    case WM_CLOSE:
//        DestroyWindow(window_handle);
//        return 0;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//    case WM_CHAR:
//        return 0;
//    //case WM_PAINT:
//    //    return 0;
//    case WM_KEYDOWN: // wparam for keycodes, lparam for info about scancode and info about counts 
//        if (wparam == VK_ESCAPE)
//        {
//            PostMessageW(window_handle, WM_CLOSE, 0, 0);
//            return 0;
//        }
//        return 0;
//    case WM_COMMAND: // for gui menu commands
//        return 0;
//    case WM_LBUTTONDOWN:
//    case WM_LBUTTONUP:
//    case WM_SIZE:
//    case WM_MOUSEWHEEL:
//        break;
//    default: 
//        return DefWindowProcW(window_handle, message, wparam, lparam);
//    }
//    return 0;
//}
//
//int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
//     _In_ PWSTR pCmdLine, _In_ int nCmdShow)
//{
//
//    WNDCLASSEXW MainWindow = { 0 };
//
//    MainWindow.cbSize = sizeof(WNDCLASSEXW),
//    MainWindow.style = CS_OWNDC;
//    MainWindow.lpfnWndProc = WindowProcedure; //DefWindowProcW
//    MainWindow.cbClsExtra = 0;
//    MainWindow.cbWndExtra = 0;
//    MainWindow.hInstance = GetModuleHandleW(nullptr);
//    MainWindow.hIcon = nullptr;
//    MainWindow.hCursor = nullptr;
//    MainWindow.hbrBackground = nullptr;
//    MainWindow.lpszMenuName = nullptr;
//    MainWindow.lpszClassName = L"WindowName";
//    MainWindow.hIconSm = nullptr;
//
//    if (!RegisterClassExW(&MainWindow))
//        return 1;
//
//    HWND window_handle = CreateWindowExW(0, MainWindow.lpszClassName, L"Window",
//        WS_OVERLAPPEDWINDOW/*(window_style)*/, CW_USEDEFAULT, CW_USEDEFAULT,
//        600, 400, nullptr, nullptr, hInstance, info); // last parameter in this function is a void pointer, so 
//    //it can be struct for store some data
//
//    if (!window_handle) {
//        return 1;
//    }
//
//    MessageBoxExW(nullptr, L"Write some text", L"Caption", MB_OK, 0);
//
//    ShowWindowAsync(window_handle, SW_SHOWDEFAULT);
//    UpdateWindow(window_handle);
//
//    MSG message {};
//    while (GetMessageW(&message, nullptr, 0, 0) > 0)
//    {
//        TranslateMessage(&message);
//        DispatchMessageW(&message);
//    }
//    return static_cast<int>(message.wParam);
//}
