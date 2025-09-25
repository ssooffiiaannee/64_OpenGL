#include <windows.h>

// g++ custom_title_bar.cpp -o custom_title_bar.exe -lgdi32

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_NCHITTEST: {
        // Allow dragging the window
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ScreenToClient(hwnd, &pt);
        RECT titleBarRect = { 0, 0, 800, 30 }; // Example title bar area
        if (PtInRect(&titleBarRect, pt)) {
            return HTCAPTION; // Allow dragging in the title bar area
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw custom title bar
        RECT rect = { 0, 0, 800, 30 }; // Title bar rectangle
        FillRect(hdc, &rect, (HBRUSH)(COLOR_ACTIVECAPTION + 1));
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, "Custom Title Bar", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CustomTitleBarWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        "CustomTitleBarWindowClass",    // Window class
        "   Custom Title Bar Example",     // Window text
        WS_OVERLAPPEDWINDOW & ~WS_CAPTION, // Window style without title bar
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
