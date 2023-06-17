document.onreadystatechange = () => {
    if (document.readyState === "complete") {
        onkeydown = (e) => {
            if (e.ctrlKey && e.key === "w") {
                window.chrome.webview.postMessage("close");
            } else {
                window.chrome.webview.postMessage(e.key);
            }
            let scheme = document.createElement("meta");
            scheme.setAttribute("name", "color-scheme");
            scheme.setAttribute("content", "light dark");
            document.getElementsByTagName("head")[0].appendChild(scheme);
            document.documentElement.style.setProperty(
                "color-scheme",
                "light dark"
            );
        };
    }
};
