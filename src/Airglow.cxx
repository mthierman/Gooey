#include "Airglow.hxx"

int __stdcall wWinMain(HINSTANCE hinstance, HINSTANCE hpinstance, PWSTR pcl, int ncs)
{
    HWND hwnd;
    auto pSettings = Settings::Create();

    if (!std::filesystem::exists(pSettings->pathData))
    {
        std::string error = "Data folder not found";
        Utility::error(error);
        return 0;
    }

    auto pWindow = MainWindow::Create(hinstance, ncs, pSettings.get());
    hwnd = pWindow.get()->m_hWnd;

    if (!hwnd)
    {
        std::string error = "Window creation failed";
        Utility::error(error);
        return 0;
    }

    pWindow->Show(hwnd, ncs);

    auto pWebView = WebView::Create(hwnd, pSettings.get());

    WebView::Initialize(hwnd);

#ifdef _DEBUG
    // Utility::Tests(hwnd);
#endif

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}