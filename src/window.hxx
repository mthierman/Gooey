// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <fstream>

#include <glow/glow.hxx>

#include "global.hxx"
#include "browser.hxx"

struct Window : public glow::window::BaseWindow<Window>
{
    using glow::window::BaseWindow<Window>::BaseWindow;

    Window(HWND app, std::string mainUrl = "about:blank", std::string sideUrl = "about:blank");

    auto default_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_close(WPARAM wParam, LPARAM lParam) -> int;
    auto on_key_down(WPARAM wParam, LPARAM lParam) -> int;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;
    auto on_size(WPARAM wParam, LPARAM lParam) -> int;

    static auto EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto url_path() -> std::string;

    auto data_path() -> std::filesystem::path;
    auto json_path() -> std::filesystem::path;
    auto save_settings() -> void;
    auto load_settings() -> void;

    std::filesystem::path m_settingsFile{json_path()};

    constexpr static int s_border{2};
    int m_bar{0};

    HWND m_app{nullptr};
    std::unique_ptr<MainBrowser> m_main;
    std::unique_ptr<SideBrowser> m_side;
    std::unique_ptr<URLBrowser> m_url;

    std::string m_mainUrl;
    std::string m_sideUrl;

    WindowDimensions m_dimensions;
};
