// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/window.hxx>

namespace airglow
{

Window::Window(HWND app, std::string name) : m_app{app} {}

auto Window::operator()(bool show) -> void
{
    glow::window::Window::operator()(show);
    dwm_caption_color(false);
    dwm_dark_mode(true);
    dwm_system_backdrop(DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW);

    m_browser1 = std::make_unique<Browser>(+Browsers::browser1, m_hwnd.get());
    (*m_browser1)();

    m_browser2 = std::make_unique<Browser>(+Browsers::browser2, m_hwnd.get());
    (*m_browser2)();

    m_bar1 = std::make_unique<Browser>(+Browsers::bar1, m_hwnd.get());
    (*m_bar1)();

    m_bar2 = std::make_unique<Browser>(+Browsers::bar2, m_hwnd.get());
    (*m_bar2)();
}

auto Window::handle_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CREATE: return on_create(hWnd, wParam, lParam);
    case WM_CLOSE: return on_close(hWnd, wParam, lParam);
    case WM_SIZE: return on_size(hWnd, wParam, lParam);
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

auto Window::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CREATE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    return 0;
}

auto Window::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    NMHDR nmhdr;
    nmhdr.code = CUSTOM_CLOSE_WINDOW;
    nmhdr.hwndFrom = m_hwnd.get();
    nmhdr.idFrom = m_id;
    SendMessageA(m_app, WM_NOTIFY, nmhdr.idFrom, std::bit_cast<LPARAM>(&nmhdr));

    m_hwnd.reset();

    return 0;
}

auto Window::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) -> int
{
    RECT rect{0};
    GetClientRect(hWnd, &rect);

    auto position{glow::window::rect_to_position(rect)};

    auto panelHeight{100};
    auto border{2};
    auto width{(position.width / 2) - border};
    auto height{(position.height) - panelHeight};
    auto rightX{width + (border * 2)};
    auto panelY{position.height - panelHeight};

    EnumChildWindows(hWnd, EnumChildProc, std::bit_cast<LPARAM>(&position));

    Sleep(1);

    return 0;
}

auto CALLBACK Window::EnumChildProc(HWND hWnd, LPARAM lParam) -> BOOL
{
    auto gwlId{static_cast<int64_t>(GetWindowLongPtrA(hWnd, GWL_ID))};
    auto position{std::bit_cast<glow::window::Position*>(lParam)};

    auto panelHeight{100};
    auto border{2};
    auto width{(position->width / 2) - border};
    auto height{(position->height) - panelHeight};
    auto rightX{width + (border * 2)};
    auto panelY{position->height - panelHeight};

    if (gwlId == +Browsers::browser1)
        SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOZORDER);

    if (gwlId == +Browsers::browser2)
        SetWindowPos(hWnd, nullptr, rightX, 0, width, height, SWP_NOZORDER);

    if (gwlId == +Browsers::bar1)
        SetWindowPos(hWnd, nullptr, 0, panelY, width, panelHeight, SWP_NOZORDER);

    if (gwlId == +Browsers::bar2)
        SetWindowPos(hWnd, nullptr, rightX, panelY, width, panelHeight, SWP_NOZORDER);

    return TRUE;
}

} // namespace airglow
