#pragma once

#include "Utility.hxx"

using namespace Utility;

struct Window
{
    HWND hwnd;
    wstring name{to_wide(APP_NAME)};
    wstring version{to_wide(APP_NAME)};
    HBRUSH darkBrush{(HBRUSH)GetStockObject(BLACK_BRUSH)};
    HBRUSH lightBrush{(HBRUSH)GetStockObject(WHITE_BRUSH)};
    HCURSOR cursor{(HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED)};
    HICON icon{(HICON)LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                 LR_SHARED | LR_DEFAULTCOLOR | LR_DEFAULTSIZE)};
};

struct Settings
{
    string theme{"dark"};
    string mainUrl{"www.google.com"};
    string sideUrl{"www.bing.com"};
    std::vector<int> position{0, 0, 0, 0};
    bool menu{false};
    bool split{false};
    bool swapped{false};
    bool maximized{false};
    bool fullscreen{false};
    bool topmost{false};
};

struct Paths
{
    path data{path_portable()};
    path settings{path_settings()};
    path json{path_json()};
    path db{path_db()};
    path js{path_js()};
    path inject{path_inject()};
};

struct Colors
{
    string accent{system_color(winrt::Windows::UI::ViewManagement::UIColorType::Accent)};
    string accentDark1{system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1)};
    string accentDark2{system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2)};
    string accentDark3{system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3)};
    string accentLight1{
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1)};
    string accentLight2{
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2)};
    string accentLight3{
        system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3)};
    string Background{system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background)};
    string Foreground{system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground)};

    Colors SystemColors()
    {
        Colors c;
        c.accent = system_color(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
        c.accentDark1 = system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark1);
        c.accentDark2 = system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark2);
        c.accentDark3 = system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentDark3);
        c.accentLight1 =
            system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight1);
        c.accentLight2 =
            system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight2);
        c.accentLight3 =
            system_color(winrt::Windows::UI::ViewManagement::UIColorType::AccentLight3);
        c.Background = system_color(winrt::Windows::UI::ViewManagement::UIColorType::Background);
        c.Foreground = system_color(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);

        return c;
    }
};
