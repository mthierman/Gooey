// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#pragma once

#include <bit>
#include <memory>

#include <nlohmann/json.hpp>

#include <config/airglow.hxx>
#include <filesystem/filesystem.hxx>
#include <gui/gui.hxx>
#include <gui/mainwindow.hxx>

#include "Browser.hxx"
#include "Settings.hxx"
#include "SettingsWindow.hxx"

using namespace glow::gui;

struct App final : public MainWindow
{
    using MainWindow::MainWindow;

    static auto run() -> int;
    auto save() -> void;
    auto load() -> void;

    auto handle_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    static auto enum_child_proc(HWND hWnd, LPARAM lParam) -> BOOL;

    auto on_key_down(WPARAM wParam) -> int;
    auto on_show_window(WPARAM wParam, LPARAM lParam) -> int;
    auto on_size() -> int;

    std::unique_ptr<Browser> m_browser1;
    std::unique_ptr<Browser> m_browser2;
    std::unique_ptr<Browser> m_browser3;
    std::unique_ptr<Browser> m_browser4;

    std::unique_ptr<Settings> m_settings;
    std::unique_ptr<SettingsWindow> m_settingsWindow;
};
