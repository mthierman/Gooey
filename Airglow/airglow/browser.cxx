// clang-format off
// ╔─────────────────────╗
// │ ╔═╗╦╦═╗╔═╗╦  ╔═╗╦ ╦ │  Airglow - https://github.com/mthierman/Airglow
// │ ╠═╣║╠╦╝║ ╦║  ║ ║║║║ │  SPDX-FileCopyrightText: © 2023 Mike Thierman <mthierman@gmail.com>
// │ ╩ ╩╩╩╚═╚═╝╩═╝╚═╝╚╩╝ │  SPDX-License-Identifier: MIT
// ╚─────────────────────╝
// clang-format on

#include <airglow/browser.hxx>

namespace airglow
{

auto Browser::accelerator_key_pressed_handler(ICoreWebView2Controller* sender,
                                              ICoreWebView2AcceleratorKeyPressedEventArgs* args)
    -> HRESULT
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

    return S_OK;
}

auto URL::initialized() -> void
{
    m_webView.m_settings8->put_AreDefaultContextMenusEnabled(false);
    m_webView.m_settings8->put_IsZoomControlEnabled(true);
    m_webView.m_core20->OpenDevToolsWindow();
}

auto URL::web_message_received_handler(ICoreWebView2* sender,
                                       ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
{
    // https://github.com/microsoft/wil/wiki/RAII-resource-wrappers
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/win32-api-conventions
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2?view=webview2-1.0.2210.55#add_webmessagereceived
    // https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2webmessagereceivedeventargs?view=webview2-1.0.2210.55#trygetwebmessageasstring
    // https://github.com/nlohmann/json/issues/1592

    // Create notification message structure

    // Check message source
    wil::unique_cotaskmem_string source;
    if (FAILED(args->get_Source(&source))) return S_OK;
    if (std::wstring_view(source.get()) != std::wstring_view(m_source)) return S_OK;

    // Get message as JSON
    wil::unique_cotaskmem_string messageRaw;
    auto asJson{args->get_WebMessageAsJson(&messageRaw)};
    if (asJson == E_INVALIDARG) return S_OK;
    if (FAILED(asJson)) return S_OK;

    // Parse the JSON and send the URL
    std::string message{glow::text::narrow(messageRaw.get())};
    try
    {
        auto parseMsg{nlohmann::json::parse(message)};

        NotificationMsg nMsg;
        nMsg.nmhdr.hwndFrom = m_hwnd.get();
        nMsg.nmhdr.idFrom = m_id;

        if (parseMsg.contains("mainUrl"))
        {
            auto mainUrl{parseMsg["mainUrl"].get<std::string>()};
            // glow::console::debug(mainUrl);
            nMsg.nmhdr.code = CUSTOM_POST_MAINURL;
            nMsg.message = mainUrl;
            SendMessageA(m_parent, WM_NOTIFY, nMsg.nmhdr.idFrom, std::bit_cast<LPARAM>(&nMsg));
        }

        if (parseMsg.contains("sideUrl"))
        {
            auto sideUrl{parseMsg["sideUrl"].get<std::string>()};
            // glow::console::debug(sideUrl);
            nMsg.nmhdr.code = CUSTOM_POST_SIDEURL;
            nMsg.message = sideUrl;
            SendMessageA(m_parent, WM_NOTIFY, nMsg.nmhdr.idFrom, std::bit_cast<LPARAM>(&nMsg));
        }

        if (parseMsg.contains("height"))
        {
            auto height{parseMsg["height"].get<int>()};
            // glow::console::debug(std::to_string(height));
            nMsg.nmhdr.code = CUSTOM_POST_HEIGHT;
            nMsg.message = std::to_string(height);
            SendMessageA(m_parent, WM_NOTIFY, nMsg.nmhdr.idFrom, std::bit_cast<LPARAM>(&nMsg));
        }
    }
    catch (const nlohmann::json::parse_error& e)
    {
        glow::console::debug(e.what());
    }

    return S_OK;
}

auto MainBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_MAINURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto MainBrowser::navigation_starting_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationStartingEventArgs* args)
    -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_MAINURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto SideBrowser::source_changed_handler(ICoreWebView2* sender,
                                         ICoreWebView2SourceChangedEventArgs* args) -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_SIDEURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

auto SideBrowser::navigation_starting_handler(ICoreWebView2* sender,
                                              ICoreWebView2NavigationStartingEventArgs* args)
    -> HRESULT
{
    glow::console::debug("navigation completed..");
    wil::unique_cotaskmem_string uriRaw;
    sender->get_Source(&uriRaw);

    auto uri{glow::text::narrow(uriRaw.get())};
    glow::console::debug(uri);
    NotificationMsg nMsg;
    nMsg.nmhdr.hwndFrom = m_hwnd.get();
    nMsg.nmhdr.idFrom = m_id;
    nMsg.nmhdr.code = CUSTOM_RECEIVE_SIDEURL;
    nMsg.message = uri;

    SendMessage(m_parent, WM_NOTIFY, 0, std::bit_cast<LPARAM>(&nMsg));

    return S_OK;
}

} // namespace airglow
