// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include "app.hxx"

App::App()
{
    SetEnvironmentVariableA("WEBVIEW2_DEFAULT_BACKGROUND_COLOR", "0");
    SetEnvironmentVariableA("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS",
                            "--allow-file-access-from-files");

    if (!std::filesystem::exists(file())) { save(); }

    else { load(); }

    parse_args();

    m_settings = std::make_unique<Settings>(hwnd(), m_state);
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
    auto notification{reinterpret_cast<glow::Notification*>(lParam)};

    auto& id{notification->id};
    auto& code{notification->code};

    switch (code)
    {
        using enum CODE;

        case SETTINGS_TOGGLE:
        {
            if (!m_settings->visible())
            {
                m_settings->show();
                m_settings->m_browser->focus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
            }

            else
            {
                if (GetForegroundWindow() != m_settings->hwnd())
                {
                    SetForegroundWindow(m_settings->hwnd());
                    m_settings->m_browser->focus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
                }

                else { m_settings->hide(); }
            }

            break;
        }

        case SETTINGS_SAVE:
        {
            save();

            break;
        }

        case WINDOW_NEW:
        {
            new_window();

            break;
        }

        case WINDOW_CLOSE:
        {
            m_windows.erase(id);

            if (m_windows.empty())
            {
                save();

                return close();
            }

            break;
        }
    }

    return 0;
}

auto App::parse_args() -> void
{
    auto argv{glow::cmd_to_argv()};

    if (argv.size() == 2) { m_state.args.first = argv.at(1); }

    else if (argv.size() > 2)
    {
        m_state.args.first = argv.at(1);
        m_state.args.second = argv.at(2);
    }
}

auto App::new_window() -> void
{
    auto id{glow::random<uintptr_t>()};
    m_windows.try_emplace(id, std::make_unique<Window>(hwnd(), id));
    m_windows.at(id)->reveal();
}

auto App::data() -> std::filesystem::path
{
    auto path{glow::known_folder() / "Airglow"};

    if (!std::filesystem::exists(path)) { std::filesystem::create_directory(path); }

    return path;
}

auto App::file() -> std::filesystem::path
{
    return std::filesystem::path{{glow::app_path() / "Airglow.json"}};
}

auto App::save() -> void
{
    try
    {
        std::ofstream f(file());
        f << std::setw(4) << nlohmann::json(m_state) << std::endl;
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
}

auto App::load() -> void
{
    try
    {
        std::ifstream f(file());
        m_state = nlohmann::json::parse(f, nullptr, false, true);
        f.close();
    }
    catch (const std::exception& e)
    {
        return;
    }
}
