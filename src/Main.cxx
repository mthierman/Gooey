#include "App.hxx"
#include "Utility.hxx"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    auto gdiplusToken{util::startup()};

    auto debugConsole{util::create_console(false)};

    auto app{std::make_unique<App>(hInstance, pCmdLine, nCmdShow)};

    if (!app)
        return util::error("Application failed to start");

    MSG msg;
    int r;

    while ((r = GetMessageW(&msg, nullptr, 0, 0)) != 0)
    {
        if (r == -1)
            return 0;

        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    util::shutdown(gdiplusToken);

    util::remove_console(debugConsole);

    return 0;
}
