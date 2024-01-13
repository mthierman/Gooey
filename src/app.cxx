// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

auto App::run() -> int
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    auto app{std::make_unique<App>()};

    auto argv = glow::console::argv();

    if (argv.size() == 2) { app->m_mainUrl = argv.at(1); }

    if (argv.size() > 2)
    {
        app->m_mainUrl = argv.at(1);
        app->m_sideUrl = argv.at(2);
    }

    // for (auto i = 0; i < 4; i++)
    // {
    //     app->m_windowVector.emplace_back(std::make_unique<Window>(app->hwnd()))->reveal();
    // }

    auto settings{std::make_unique<Settings>(app->hwnd())};
    settings->reveal();

    return glow::gui::message_loop();
}

auto App::wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_NOTIFY: return on_notify(wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto App::on_notify(WPARAM wParam, LPARAM lParam) -> int
{
    auto nmhdr{*std::bit_cast<LPNMHDR>(lParam)};

    switch (nmhdr.code)
    {
    case msg::window_create: m_windowSet.insert(nmhdr.idFrom); break;
    case msg::window_close: m_windowSet.erase(nmhdr.idFrom); break;
    }

    if (m_windowSet.empty()) { return close(); }

    else return 0;
}

auto App::data_path() -> std::filesystem::path
{
    auto path{glow::filesystem::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);

    return path;
}

auto App::json_path() -> std::filesystem::path
{
    auto path{data_path() / "Airglow.json"};

    return path;
}

// auto App::save_settings() -> void
// {
//     auto path{filesystem::portable()};
//     if (!path.empty())
//     {
//         auto settingsFile{path / "settings.json"};

//         try
//         {
//             if (!std::filesystem::exists(settingsFile))
//             {
//                 try
//                 {
//                     nlohmann::json j = *this;
//                     std::ofstream f(settingsFile);
//                     f << std::setw(4) << j << "\n";
//                     f.close();
//                 }
//                 catch (const std::exception& e)
//                 {
//                     std::println("{}", e.what());
//                 }
//             }
//             else
//             {
//                 try
//                 {
//                     nlohmann::json j = *this;
//                     std::ofstream f(settingsFile);
//                     f << std::setw(4) << j << "\n";
//                     f.close();
//                 }
//                 catch (const std::exception& e)
//                 {
//                     std::println("{}", e.what());
//                 }
//             }
//         }
//         catch (const std::filesystem::filesystem_error& e)
//         {
//             std::println("{}", e.what());
//         }
//     }
// }

// auto App::load_settings() -> void
// {
//     auto path{glow::filesystem::portable()};
//     if (!path.empty())
//     {
//         auto settingsFile{path / "settings.json"};

//         try
//         {
//             if (!std::filesystem::exists(settingsFile))
//             {
//                 try
//                 {
//                     nlohmann::json j = *this;
//                     std::ofstream f(settingsFile);
//                     f << std::setw(4) << j << "\n";
//                     f.close();
//                 }
//                 catch (const std::exception& e)
//                 {
//                     std::println("{}", e.what());
//                 }
//             }
//             else
//             {
//                 try
//                 {
//                     //
//                 }
//                 catch (const std::exception& e)
//                 {
//                     std::println("{}", e.what());
//                 }
//             }
//         }
//         catch (const std::filesystem::filesystem_error& e)
//         {
//             std::println("{}", e.what());
//         }
//     }
// }
