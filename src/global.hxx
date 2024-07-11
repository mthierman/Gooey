// clang-format off
// Airglow - https://github.com/mthierman/Airglow
// SPDX-FileCopyrightText: © 2024 Mike Thierman <mthierman@gmail.com>
// SPDX-License-Identifier: MIT
// clang-format on

#pragma once

#include <Windows.h>

#include <set>

enum struct CODE : unsigned int {
    SETTINGS_TOGGLE = WM_APP,
    SETTINGS_SAVE,
    WINDOW_NEW,
    WINDOW_CLOSE,
    WINDOW_ACTIVATE,
    SETTING_CHANGE,
    DPI_CHANGE,
    LAYOUT_CHANGE,
    WEB_MESSAGE_RECEIVED,
    SOURCE_CHANGED,
    FAVICON_CHANGED,
    TITLE_CHANGED,
    HOME_CHANGED,
    FOCUS_CHANGED
};

namespace glow::messages {
enum struct notice : unsigned int {
    CREATE_WINDOW = WM_APP,
    CREATE_FOREGROUND_WINDOW,
    CLOSE_WINDOW,
};
}

struct Keys {
    Keys();

    std::set<unsigned int> set {};
};
