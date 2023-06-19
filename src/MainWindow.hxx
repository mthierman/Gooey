#pragma once

#include "Settings.hxx"

class MainWindow
{
  public:
    static std::unique_ptr<MainWindow> Create(HINSTANCE, int);
    static __int64 __stdcall _WndProc(HWND, UINT, WPARAM, LPARAM);
    static bool _ShowWindow(HWND, int);
    HWND m_hWnd;
    std::unique_ptr<Settings> settings;

    // MESSAGES
    int _OnCommand();
    int _OnCreate(HWND);
    int _OnDestroy();
    int _OnDpiChanged();
    int _OnGetMinMaxInfo(LPARAM);
    int _OnPaint(HWND);
    int _OnSize();
    int _OnWindowPosChanged(HWND);
    int _OnSettingChange(HWND);
    int _OnKeyDown(HWND, WPARAM);

    // THEME
    bool CheckSystemDarkMode();
    bool SetDarkTitle();
    bool SetDarkMode(HWND);
    bool SetMica(HWND);

    // STATE
    static std::pair<std::wstring, std::wstring> CommandLine();
    bool Toggle(bool);
    void MaximizeWindow(HWND);
    void FullscreenWindow(HWND);
    void TopmostWindow(HWND);
    void UpdateFocus();
    void UpdateBounds(HWND);
    static RECT GetFullBounds(HWND);
    static RECT GetMenuBounds(HWND);
    static RECT GetMainPanelBounds(HWND);
    static RECT GetSidePanelBounds(HWND);

  private:
    static unsigned long long gdiplusToken;
    static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    MainWindow(HINSTANCE, int);
    enum PreferredAppMode
    {
        Default,
        AllowDark,
        ForceDark,
        ForceLight,
        Max
    };
};
