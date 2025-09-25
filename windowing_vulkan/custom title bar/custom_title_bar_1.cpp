#include <windows.h>

const int TITLE_BAR_HEIGHT = 30; // Height of the custom title bar

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_NCHITTEST: {
        // Handle window dragging
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ScreenToClient(hwnd, &pt);

        // Check if the click is in the custom title bar area
        if (pt.y < TITLE_BAR_HEIGHT) {
            return HTCAPTION; // Allow dragging in the title bar area
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw custom title bar
        RECT rect = { 0, 0, ps.rcPaint.right, TITLE_BAR_HEIGHT };
        FillRect(hdc, &rect, (HBRUSH)(COLOR_ACTIVECAPTION + 1)); // Title bar background color
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

    // Create a window without the default title bar
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        "CustomTitleBarWindowClass",    // Window class
        "Custom Title Bar Example",     // Window text
        WS_OVERLAPPEDWINDOW & ~WS_CAPTION, // Remove the title bar
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
