#include "Gooey.hpp"

ATOM Application(HINSTANCE hinstance)
{
    auto icon = (HICON)LoadImageW(hinstance, programIcon.c_str(), IMAGE_ICON, 0, 0,
                                  LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED);

    auto cursor = (HCURSOR)LoadImageW(nullptr, (LPCWSTR)IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);

    auto hollowBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hinstance;
    wcex.hCursor = cursor;
    wcex.hbrBackground = hollowBrush;
    wcex.lpszMenuName = menuName.c_str();
    wcex.lpszClassName = className.c_str();
    wcex.hIcon = icon;
    wcex.hIconSm = icon;

    return RegisterClassExW(&wcex);
}

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    SetEnvironmentVariableW(wvBackgroundColor.c_str(), wvBackgroundColorValue.c_str());

    SetEnvironmentVariableW(wvAdditionalBrowserArgs.c_str(), wvAdditionalBrowserArgsValue.c_str());

    Application(hinstance);

    auto hwnd = CreateWindowExW(0, className.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
                                nullptr, hinstance, nullptr);

    if (!hwnd)
    {
        return 0;
    }

    systemDarkMode = CheckSystemDarkMode();
    darkTitle = SetDarkTitle();
    darkMode = SetDarkMode(hwnd);
    // mica = SetMica(hwnd);

    ShowWindow(hwnd, ncs);

    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                env->CreateCoreWebView2Controller(
                    hwnd,
                    Microsoft::WRL::Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
                        {
                            if (controller != nullptr)
                            {
                                wv_controller = controller;
                                wv_controller->get_CoreWebView2(&wv);
                            }

                            wv->get_Settings(&wv_settings);

                            wv_settings->put_AreDefaultContextMenusEnabled(true);
                            wv_settings->put_AreDefaultScriptDialogsEnabled(true);
                            wv_settings->put_AreDevToolsEnabled(true);
                            wv_settings->put_AreHostObjectsAllowed(true);
                            wv_settings->put_IsBuiltInErrorPageEnabled(true);
                            wv_settings->put_IsScriptEnabled(true);
                            wv_settings->put_IsStatusBarEnabled(true);
                            wv_settings->put_IsWebMessageEnabled(true);
                            wv_settings->put_IsZoomControlEnabled(true);

                            RECT bounds;
                            RECT wvRect;

                            // GetClientRect(hwnd, &bounds);

                            // wvRect = {
                            //     bounds.left,
                            //     bounds.top,
                            //     bounds.right / 2,
                            //     bounds.bottom,
                            // };

                            // wv_controller->put_Bounds(wvRect);

                            GetClientRect(hwnd, &bounds);

                            wv_controller->put_Bounds(bounds);

                            WebViewNavigate(wv);

                            EventRegistrationToken token;

                            wv->ExecuteScript(wvScript.c_str(), nullptr);

                            wv->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);

                            wv->add_WebMessageReceived(
                                Microsoft::WRL::Callback<
                                    ICoreWebView2WebMessageReceivedEventHandler>(
                                    [hwnd](
                                        ICoreWebView2* webview,
                                        ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
                                    {
                                        wil::unique_cotaskmem_string message;

                                        args->TryGetWebMessageAsString(&message);

                                        if ((std::wstring)message.get() == keyTop.c_str())
                                        {
                                            KeyTop(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyMax.c_str())
                                        {
                                            KeyMaximize(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyFull.c_str())
                                        {
                                            KeyFullscreen(hwnd);
                                        }

                                        if ((std::wstring)message.get() == keyClose.c_str())
                                        {
                                            KeyClose(hwnd);
                                        }

                                        webview->PostWebMessageAsString(message.get());

                                        return S_OK;
                                    })
                                    .Get(),
                                &token);
                            return S_OK;
                        })
                        .Get());
                return S_OK;
            })
            .Get());

    // CreateCoreWebView2EnvironmentWithOptions(
    //     nullptr, nullptr, nullptr,
    //     Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
    //         [hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
    //         {
    //             env->CreateCoreWebView2Controller(
    //                 hwnd,
    //                 Microsoft::WRL::Callback<
    //                     ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
    //                     [hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
    //                     {
    //                         if (controller != nullptr)
    //                         {
    //                             wv_controller2 = controller;
    //                             wv_controller2->get_CoreWebView2(&wv2);
    //                         }

    //                         wv2->get_Settings(&wv_settings2);

    //                         wv_settings2->put_AreDefaultContextMenusEnabled(true);
    //                         wv_settings2->put_AreDefaultScriptDialogsEnabled(true);
    //                         wv_settings2->put_AreDevToolsEnabled(true);
    //                         wv_settings2->put_AreHostObjectsAllowed(true);
    //                         wv_settings2->put_IsBuiltInErrorPageEnabled(true);
    //                         wv_settings2->put_IsScriptEnabled(true);
    //                         wv_settings2->put_IsStatusBarEnabled(true);
    //                         wv_settings2->put_IsWebMessageEnabled(true);
    //                         wv_settings2->put_IsZoomControlEnabled(true);

    //                         RECT bounds2;
    //                         RECT wvRect2;

    //                         GetClientRect(hwnd, &bounds2);

    //                         wvRect2 = {
    //                             bounds2.right / 2,
    //                             bounds2.top,
    //                             bounds2.right,
    //                             bounds2.bottom,
    //                         };

    //                         wv_controller2->put_Bounds(wvRect2);

    //                         WebViewNavigate(wv2);

    //                         EventRegistrationToken token;

    //                         wv2->ExecuteScript(wvScript.c_str(), nullptr);

    //                         wv2->AddScriptToExecuteOnDocumentCreated(wvScript.c_str(), nullptr);

    //                         wv2->add_WebMessageReceived(
    //                             Microsoft::WRL::Callback<
    //                                 ICoreWebView2WebMessageReceivedEventHandler>(
    //                                 [hwnd](
    //                                     ICoreWebView2* webview,
    //                                     ICoreWebView2WebMessageReceivedEventArgs* args) ->
    //                                     HRESULT
    //                                 {
    //                                     wil::unique_cotaskmem_string message;

    //                                     args->TryGetWebMessageAsString(&message);

    //                                     if ((std::wstring)message.get() == keyTop.c_str())
    //                                     {
    //                                         KeyTop(hwnd);
    //                                     }

    //                                     if ((std::wstring)message.get() == keyMax.c_str())
    //                                     {
    //                                         KeyMaximize(hwnd);
    //                                     }

    //                                     if ((std::wstring)message.get() == keyFull.c_str())
    //                                     {
    //                                         KeyFullscreen(hwnd);
    //                                     }

    //                                     if ((std::wstring)message.get() == keyClose.c_str())
    //                                     {
    //                                         KeyClose(hwnd);
    //                                     }

    //                                     webview->PostWebMessageAsString(message.get());

    //                                     return S_OK;
    //                                 })
    //                                 .Get(),
    //                             &token);
    //                         return S_OK;
    //                     })
    //                     .Get());
    //             return S_OK;
    //         })
    //         .Get());

    // auto menuHandle = GetSystemMenu(hwnd, false);
    // MENUITEMINFOW menu;
    // menu.cbSize = sizeof(MENUITEMINFOW);
    // menu.fMask = MIIM_STRING | MIIM_ID;
    // menu.wID = 1005;
    // menu.dwTypeData = (LPWSTR)L"Toggle Theme";

    // InsertMenuItemW(menuHandle, 1, FALSE, &menu);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {

        // case WM_PAINT:
        // {
        //     PAINTSTRUCT ps;
        //     HDC hdc = BeginPaint(hwnd, &ps);
        //     EndPaint(hwnd, &ps);
        // }
        // break;

    case WM_SETFOCUS:
    {
        if (wv_controller != nullptr)
        {
            wv_controller->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
        if (wv_controller2 != nullptr)
        {
            wv_controller2->MoveFocus(
                COREWEBVIEW2_MOVE_FOCUS_REASON::COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
    }
    break;

    case WM_SETTINGCHANGE:
    {
        using namespace winrt::Windows::UI;
        using namespace winrt::Windows::UI::ViewManagement;

        // auto accentCheck = UISettings().GetColorValue(UIColorType::Accent);
        // COLORREF darkTitleBar = RGB(32, 32, 32);
        // COLORREF lightTitleBar = RGB(243, 243, 243);

        systemDarkMode = CheckSystemDarkMode();
        InvalidateRect(hwnd, NULL, true);
        darkMode = SetDarkMode(hwnd);

        // if (mica & darkMode)
        // {
        //     DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &darkTitleBar, sizeof(darkTitleBar));
        //     DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &darkTitleBar,
        //     sizeof(darkTitleBar));
        // }

        // if (mica & !darkMode)
        // {
        //     DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &lightTitleBar,
        //     sizeof(lightTitleBar)); DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR,
        //     &lightTitleBar, sizeof(lightTitleBar));
        // }
    }
    break;

    case WM_SIZE:
    {
        // if (wv_controller != nullptr & wv_controller2 != nullptr)
        // {
        //     if (wv_controller != nullptr)
        //     {
        //         RECT bounds;
        //         GetClientRect(hwnd, &bounds);
        //         RECT wvRect = {
        //             bounds.left,
        //             bounds.top,
        //             bounds.right / 2,
        //             bounds.bottom,
        //         };
        //         wv_controller->put_Bounds(wvRect);
        //     }
        //     if (wv_controller2 != nullptr)
        //     {
        //         RECT bounds2;
        //         GetClientRect(hwnd, &bounds2);
        //         RECT wvRect2 = {
        //             bounds2.right / 2,
        //             bounds2.top,
        //             bounds2.right,
        //             bounds2.bottom,
        //         };
        //         wv_controller2->put_Bounds(wvRect2);
        //     }
        // }

        if (wv_controller != nullptr)
        {
            RECT bounds;
            GetClientRect(hwnd, &bounds);
            wv_controller->put_Bounds(bounds);
        }

        Debug(hwnd);
    }
    break;

    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO minmax = (LPMINMAXINFO)lp;
        minmax->ptMinTrackSize.x = 300;
        minmax->ptMinTrackSize.y = 39;
    }
    break;

    case WM_KEYDOWN:
    {
        if (wp == vkKeyTop)
        {
            KeyTop(hwnd);
        }
        if (wp == vkKeyMax)
        {
            KeyMaximize(hwnd);
        }
        if (wp == vkKeyFull)
        {
            KeyFullscreen(hwnd);
        }
        if (wp == vkKeyW)
        {
            auto state = GetKeyState(vkKeyControl);
            if (state & 0x8000)
            {
                KeyClose(hwnd);
            }
        }
    }
    break;

    case WM_CLOSE:
    {
        DestroyWindow(hwnd);
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;

    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }

    return 0;
}

bool CheckSystemDarkMode()
{
    auto settingsCheck = winrt::Windows::UI::ViewManagement::UISettings();
    auto fgCheck =
        settingsCheck.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);
    return (((5 * fgCheck.G) + (2 * fgCheck.R) + fgCheck.B) > (8 * 128));
}

bool SetDarkTitle()
{
    using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);
    auto uxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (uxtheme)
    {
        auto ord135 = GetProcAddress(uxtheme, PCSTR MAKEINTRESOURCEW(135));
        if (ord135)
        {
            auto SetPreferredAppMode = reinterpret_cast<fnSetPreferredAppMode>(ord135);
            SetPreferredAppMode(PreferredAppMode::AllowDark);
        }
        FreeLibrary(uxtheme);
        return true;
    }
    return false;
}

bool SetDarkMode(HWND hwnd)
{
    auto dwmtrue = TRUE;
    auto dwmfalse = FALSE;

    if (systemDarkMode)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmtrue, sizeof(dwmtrue));
        return true;
    }
    if (!systemDarkMode)
    {
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dwmfalse, sizeof(dwmfalse));
    }
    return false;
}

bool SetMica(HWND hwnd)
{
    MARGINS m = {-1};
    auto extend = S_OK;
    extend = DwmExtendFrameIntoClientArea(hwnd, &m);
    if (SUCCEEDED(extend))
    {
        auto backdrop = S_OK;
        backdrop = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
        backdrop =
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(&backdrop));
        if (SUCCEEDED(backdrop))
        {
            return true;
        }
        return false;
    }
    return false;
}

void KeyTop(HWND hwnd)
{
    auto topMost = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    if (topMost & WS_EX_TOPMOST)
    {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else
    {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}

void KeyMaximize(HWND hwnd)
{
    WINDOWPLACEMENT wPlacement = {};
    wPlacement.length = sizeof(WINDOWPLACEMENT);
    auto placement = GetWindowPlacement(hwnd, &wPlacement);
    if (wPlacement.showCmd == SW_SHOWNORMAL)
    {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
    if (wPlacement.showCmd == SW_SHOWMAXIMIZED)
    {
        ShowWindow(hwnd, SW_SHOWNORMAL);
    }
}

void KeyFullscreen(HWND hwnd)
{
    auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);
    static RECT position;
    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = {sizeof(mi)};
        GetWindowRect(hwnd, &position);
        if (GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
        {
            SetWindowLongPtrW(hwnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLongPtrW(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        SetWindowPos(hwnd, nullptr, position.left, position.top, (position.right - position.left),
                     (position.bottom - position.top), 0);
    }
}

void KeyClose(HWND hwnd) { SendMessageW(hwnd, WM_CLOSE, 0, 0); }

void WebViewNavigate(wil::com_ptr<ICoreWebView2> wv)
{
    int nArgs;
    int i;
    LPWSTR commandLine = GetCommandLineW();
    LPWSTR* commandLineList = CommandLineToArgvW(commandLine, &nArgs);
    if (0 == commandLineList[1])
    {
        wv->Navigate(L"about:blank");
    }
    for (i = 1; i < nArgs; i++)
    {
        wv->Navigate(commandLineList[i]);
    }
    LocalFree(commandLineList);
}

void Debug(HWND hwnd)
{
    using namespace winrt::Windows::UI;
    using namespace winrt::Windows::UI::ViewManagement;

    // COLORREF border;

    // auto borderColor = winrt::check_hresult((hwnd, DWMWA_BORDER_COLOR, &border, sizeof(border)));
    // auto border2 = DWMWA_BORDER_COLOR;
    // auto test = std::to_wstring(borderColor.value);
    // OutputDebugStringW(test.c_str());

    // auto accent = UISettings().GetColorValue(UIColorType::Accent);
    // COLORREF accent2 = RGB(accent.R, accent.G, accent.B);

    // auto accent = UISettings().GetColorValue(UIColorType::Accent);
    // std::wstring accentFmt = L"R: " + std::to_wstring(accent.R) + L" G: " +
    //                          std::to_wstring(accent.G) + L" B: " + std::to_wstring(accent.B) +
    //                          L" A: " + std::to_wstring(accent.A);
    // OutputDebugStringW(accentFmt.c_str());
    // OutputDebugStringW(L"\n");

    // if (UISettings().AdvancedEffectsEnabled())
    // {
    //     OutputDebugStringW(L"Advanced Effects: Enabled");
    //     OutputDebugStringW(L"\n");
    // };
    // if (UISettings().AnimationsEnabled())
    // {
    //     OutputDebugStringW(L"Animations: Enabled");
    //     OutputDebugStringW(L"\n");
    // };
}
