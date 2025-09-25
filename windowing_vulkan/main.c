#include <windows.h>

// Function to process messages sent to the window
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

// Entry point of a Windows application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Define the window class
    const char CLASS_NAME[] = "Sample Window Class";
    
    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;          // Window procedure function
    wc.hInstance     = hInstance;           // Handle to application instance
    wc.lpszClassName = CLASS_NAME;          // Window class name

    // Register the window class
    RegisterClass(&wc); // Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                            // Optional window styles
        CLASS_NAME,                   // Window class name
        "Win32 API Sample",           // Window title
        WS_OVERLAPPEDWINDOW,          // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Application instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Show the window
    ShowWindow(hwnd, nCmdShow);

    // Main message loop
    MSG msg = { };
    // Retrieves a message from the calling thread's message queue. The function dispatches incoming sent messages until a posted message is available for retrieval.
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // Translates virtual-key messages into character messages. The character messages are posted to the calling thread's message queue, to be read the next time the thread calls the GetMessage or PeekMessage function
        DispatchMessage(&msg); // Dispatches a message to a window procedure. It is typically used to dispatch a message retrieved by the GetMessage function.
    }

    return 0;
}
