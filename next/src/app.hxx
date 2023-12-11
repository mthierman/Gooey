// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <gui/app.hxx>

//==============================================================================
namespace airglow
{

//==============================================================================
struct App final : public glow::gui::App
{
    using glow::gui::App::App;

  private:
    auto handle_message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;
    static auto enum_child_proc(HWND hwnd, LPARAM lParam) -> BOOL;

    //==============================================================================
    auto on_notify() -> int;
    auto on_window_pos_changed() -> int;
};

//==============================================================================
} // namespace airglow
