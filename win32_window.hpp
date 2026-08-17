#pragma once
#include <string>
// getkeystate, methods thats calls on event should be here? onMove, onSize etc.
class Window
{
public:
    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window() noexcept;

    int RunMessageLoop();
    bool Create(int width, int height, const std::wstring& title);
    void Show() const noexcept;
    void Hide() const noexcept; // use CloseWindow(hwnd)!
    //int GetWidth() const;
    //int GetHeight() const;
    //RECT GetClientRect();
    void SetTitle(const std::wstring& title) const;
    // maximize?, minimize?, close?, draw?(this may be not this class responsibility), resize?, work with dpi?,
    //void SetEventHandler(IEventHandler* handler);

private:
    struct window_info
    {
        //store prev size, state, files and etc.
    };
    //getClientWidth, getClientHeight
    //getWindowWidth, getWindowHeight
    static LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

   bool is_created {};
   static inline bool is_maximized { false };
   static inline bool is_collapsed { false };
    bool fullscreen   { false }; // store data and write it to storage file
    int client_width  {};
    int client_height {};
    HINSTANCE instance_handle {};
    HWND main_handle;
    //IEventHandler* event_handler;
};