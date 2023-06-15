#define PROGRAM_ICON 1

// MAIN
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);
__int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

// WINDOW
unsigned short MakeWindowClass(HINSTANCE);
HWND MakeWindow(HINSTANCE);
HWND InitializeWindow(HINSTANCE, int);
RECT GetWindowBounds(HWND window);
RECT GetHiddenPanelBounds(HWND window);
RECT GetFullPanelBounds(HWND window);
RECT GetLeftPanelBounds(HWND window);
RECT GetRightPanelBounds(HWND window);
RECT GetBottomPanelBounds(HWND window);
bool PanelHideMenu(HWND);
bool PanelSplit(HWND);
bool WindowMaximize(HWND);
bool WindowFullscreen(HWND);
bool WindowTop(HWND);
std::wstring programIcon(L"PROGRAM_ICON");
std::wstring className(L"window");
std::wstring windowName(L"Airglow");
std::wstring menuName(L"menu");
bool isTopmost;
bool isMaximized;
bool isFullscreen;
bool isSplit;
bool panelMenu = true;
bool panelLeft;
bool panelRight;

// GDI+
unsigned long long gdiplusToken;
Gdiplus::GdiplusStartupInput gdiplusStartupInput;

// ENV
std::wstring wvBackgroundColor(L"WEBVIEW2_DEFAULT_BACKGROUND_COLOR");
std::wstring wvBackgroundColorValue(L"0");
std::wstring wvAdditionalBrowserArgs(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS");
std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
                                          L"OverlayScrollbar,"
                                          L"msOverlayScrollbarWinStyle,"
                                          L"msOverlayScrollbarWinStyleAnimation,"
                                          L"msWebView2BrowserHitTransparent");

// DEBUG
// std::wstring wvAdditionalBrowserArgsValue(L"--enable-features="
//                                           L"OverlayScrollbar,"
//                                           L"msOverlayScrollbarWinStyle,"
//                                           L"msOverlayScrollbarWinStyleAnimation,"
//                                           L"msWebView2BrowserHitTransparent,"
//                                           L" --show-paint-rects"
//                                           L" --show-screenspace-rects"
//                                           L" --ui-show-fps-counter");

// WEBVIEW
static wil::com_ptr<ICoreWebView2Controller> wv_controller;
static wil::com_ptr<ICoreWebView2> wv_core;
static wil::com_ptr<ICoreWebView2_19> wv;
static wil::com_ptr<ICoreWebView2Settings> wv_settings;

static wil::com_ptr<ICoreWebView2Controller> wv_controller2;
static wil::com_ptr<ICoreWebView2> wv_core2;
static wil::com_ptr<ICoreWebView2_19> wv2;
static wil::com_ptr<ICoreWebView2Settings> wv_settings2;

static wil::com_ptr<ICoreWebView2Controller> wv_controller3;
static wil::com_ptr<ICoreWebView2> wv_core3;
static wil::com_ptr<ICoreWebView2_19> wv3;
static wil::com_ptr<ICoreWebView2Settings> wv_settings3;

bool CommandLineUrl();
bool GetAppDataPath();
std::filesystem::path userData;
void InitializeWebView1(HWND window, std::filesystem::path userData);
void InitializeWebView2(HWND window, std::filesystem::path userData);
void InitializeWebView3(HWND window, std::filesystem::path userData);
std::wstring wvScript(
    L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown = "
    L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} "
    L"else "
    L"{window.chrome.webview.postMessage(e.key)}}}}");
std::wstring wvScriptBottom(
    L"document.onreadystatechange = () => {if (document.readyState === 'complete') {onkeydown = "
    L"(e) => {if (e.ctrlKey && e.key === 'w') {window.chrome.webview.postMessage('close')} "
    L"if (e.key === 'F3') {e.preventDefault();} "
    L"else "
    L"{console.log(e); window.chrome.webview.postMessage(e.key)}}}}");
std::wstring url1 = L"https://www.typescriptlang.org/docs/handbook/intro.html";
std::wstring url2 = L"https://www.typescriptlang.org/play";
std::wstring url3 = L"https://localhost:8000";
// std::wstring url3 = L"about:blank";
wil::unique_cotaskmem_string title;

// THEME
bool CheckSystemDarkMode();
bool SetDarkTitle();
bool SetDarkMode(HWND);
bool SetMica(HWND);
bool SetWindow(HWND, int);
enum PreferredAppMode
{
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
};
