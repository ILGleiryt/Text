#pragma once
import std;

class Window
{
public:
    [[nodiscard]] Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window() noexcept;

    int RunMessageLoop() const noexcept;

    [[nodiscard]] bool Create(int width, int height, const std::wstring& title);

    void SetTitle(const std::wstring& title) const noexcept;

    void Destroy() const noexcept;

    void Show() const noexcept;

    void Minimize()  const noexcept;
    void Maximize() const noexcept;
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
    static LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

    int dpi = DEFAULT_DPI;
    //getClientWidth, getClientHeight
    //getWindowWidth, getWindowHeight
    bool is_created {};
    bool is_maximized { false };
    bool is_collapsed { false };
    bool fullscreen { false }; // store data and write it to storage file
    int client_width {};
    int client_height {};

    HINSTANCE instance_handle {};
    HWND main_handle {};
};