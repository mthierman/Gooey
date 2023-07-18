#pragma once

namespace Utility
{
using ifstream = std::ifstream;
using json = nlohmann::json;
using ofstream = std::ofstream;
using path = std::filesystem::path;
using string = std::string;
using stringstream = std::stringstream;
using wstring = std::wstring;

string to_string(wstring);
wstring to_wide(string);
string bool_to_string(bool);
wstring bool_to_wide(bool);
bool bool_toggle(bool);
void print(string);
void println(string);
void wprint(wstring);
void wprintln(wstring);
void msgbox(string);
void msgboxw(wstring);
void error(string);
void errorw(wstring);
void dberror(string);

path path_appdata();
path path_portable();
path path_settings();
path path_json();
path path_db();
path path_js();
path path_inject();

string system_color(winrt::Windows::UI::ViewManagement::UIColorType);

string system_theme();
string window_theme(HWND);

bool window_darktitle();
bool window_cloak(HWND);
bool window_uncloak(HWND);
bool window_mica(HWND);
bool window_maximize(HWND);
bool window_fullscreen(HWND);
bool window_topmost(HWND);
std::vector<int> window_position(HWND);

std::pair<wstring, wstring> command_line();
wstring js_inject();
wstring js_inject_embed();

std::vector<int> rect_to_bounds(RECT);
RECT bounds_to_rect(std::vector<int>);

RECT window_bounds(HWND);
RECT left_panel(RECT);
RECT right_panel(RECT);

namespace State
{
struct Window
{
    HWND hwnd{nullptr};
    wstring name{to_wide(APP_NAME)};
    wstring version{to_wide(APP_NAME)};
    HBRUSH darkBrush{(HBRUSH)GetStockObject(BLACK_BRUSH)};
    HBRUSH lightBrush{(HBRUSH)GetStockObject(WHITE_BRUSH)};
    HCURSOR cursor{(HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED)};
    HICON icon{(HICON)LoadImageW(nullptr, (LPCWSTR)IDI_APPLICATION, IMAGE_ICON, 0, 0,
                                 LR_SHARED | LR_DEFAULTCOLOR | LR_DEFAULTSIZE)};

    string theme{""};
    string mainUrl{""};
    string sideUrl{""};
    std::vector<int> position{0, 0, 0, 0};
    bool menu{};
    bool split{};
    bool swapped{};
    bool maximized{};
    bool fullscreen{};
    bool topmost{};
};
struct Path
{
    path data{path_portable()};
    path settings{path_settings()};
    path json{path_json()};
    path db{path_db()};
    path js{path_js()};
    path inject{path_inject()};
};
struct Color
{
    string accent{};
    string accentDark1{};
    string accentDark2{};
    string accentDark3{};
    string accentLight1{};
    string accentLight2{};
    string accentLight3{};
    string Background{};
    string Foreground{};
};
json window_serialize(Window);
Window window_deserialize(json);
json window_load_state(Path);
void window_save_state(Path, json);
Color system_colors();
} // namespace State
} // namespace Utility
