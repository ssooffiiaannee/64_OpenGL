#include <wms_win32.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:  // When the window is closed
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:  // When the window needs to be repainted
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw text in the window
            TextOut(hdc, 100, 100, "Hello, Win32 API!", 17);

            EndPaint(hwnd, &ps);
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}