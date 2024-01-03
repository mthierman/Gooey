// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/browser.hxx>

auto Browser::initialized() -> void
{
    // m_settings8->put_AreBrowserAcceleratorKeysEnabled(true);
    // m_settings8->put_AreDefaultContextMenusEnabled(true);
    // m_settings8->put_AreDefaultScriptDialogsEnabled(true);
    // m_settings8->put_AreDevToolsEnabled(true);
    // m_settings8->put_AreHostObjectsAllowed(true);
    // m_settings8->put_HiddenPdfToolbarItems(
    //     COREWEBVIEW2_PDF_TOOLBAR_ITEMS::COREWEBVIEW2_PDF_TOOLBAR_ITEMS_NONE);
    // m_settings8->put_IsBuiltInErrorPageEnabled(true);
    // m_settings8->put_IsGeneralAutofillEnabled(true);
    // m_settings8->put_IsPasswordAutosaveEnabled(true);
    // m_settings8->put_IsPinchZoomEnabled(true);
    // m_settings8->put_IsReputationCheckingRequired(true);
    // m_settings8->put_IsScriptEnabled(true);
    // m_settings8->put_IsStatusBarEnabled(true);
    // m_settings8->put_IsSwipeNavigationEnabled(true);
    // m_settings8->put_IsWebMessageEnabled(true);
    // m_settings8->put_IsZoomControlEnabled(true);
}

auto Browser::web_message_received_handler() -> void {}

auto Browser::accelerator_key_pressed_handler(ICoreWebView2AcceleratorKeyPressedEventArgs* args)
    -> void
{
    COREWEBVIEW2_KEY_EVENT_KIND kind{};
    args->get_KeyEventKind(&kind);

    if (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN ||
        kind == COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN)
    {
        UINT key{0};
        args->get_VirtualKey(&key);

        switch (key)
        {
        case 19:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_PAUSE, 0);
            break;

        case 76:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, 0x4C, 0);

            break;

        case 87:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, 0x57, 0);

            break;

        case 112:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F1, 0);

            break;

        case 113:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F2, 0);

            break;

        case 114:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F3, 0);

            break;

        case 115:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F4, 0);

            break;

        case 117:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F6, 0);

            break;

        case 118:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F7, 0);

            break;

        case 119:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F8, 0);

            break;

        case 120:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F9, 0);

            break;

        case 121:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F10, 0);

            break;

        case 122:
            args->put_Handled(TRUE);
            SendMessageA(m_parent, WM_KEYDOWN, VK_F11, 0);

            break;
        }
    }
}