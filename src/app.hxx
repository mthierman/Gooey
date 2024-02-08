// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <Windows.h>

#include <filesystem>
#include <map>
#include <memory>

#include <glow/glow.hxx>

#include "global.hxx"
#include "settings.hxx"
#include "window.hxx"

struct App : public glow::App<App>
{
    using glow::App<App>::App;

    auto operator()() -> int;
    auto window(uintptr_t id = glow::random<uintptr_t>()) -> void;

    auto wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto on_notify(WPARAM wParam, LPARAM lParam) -> int;

    glow::GdiPlus m_gdiInit;
    glow::CoInitialize m_coInit;

    URL m_url;
    std::unique_ptr<Settings> m_settings;
    std::unordered_map<uintptr_t, std::unique_ptr<Window>> m_windows;
};
