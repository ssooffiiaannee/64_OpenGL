#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#include <wms_win32.h>

HWND hwnd;
HINSTANCE hInstance_global;

int init_win32_window()
{
     // create a window class that store all the data about the main window
    const char CLASS_NAME[] = "test";
    WNDCLASS wc = {};
    

    wc.lpfnWndProc   = WindowProc;          // Window procedure function
    wc.hInstance     = hInstance_global;           // Handle to application instance
    wc.lpszClassName =  CLASS_NAME;          // Window class name

    RegisterClass(&wc);
    
    hwnd = CreateWindowEx(
        0,                            // Optional window styles
        CLASS_NAME, //(wchar_t *) CLASS_NAME,                   // Window class name
        "Win32 API Sample",           // Window title
        WS_OVERLAPPEDWINDOW,          // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance_global,  // Application instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }
    return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MessageBox(hwnd, "hello", "messagebox_test", MB_ABORTRETRYIGNORE);
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout); // redirect stdout to console

    FILE *fp_err;
    freopen_s(&fp_err, "file.txt", "w", stderr); 
    
    printf("testing");
    hInstance_global = hInstance;
    int res = init_win32_window();
    fprintf(stderr, "err\n", 30);
    if(res != 1){
        fprintf(stderr, "err\n");
    }
    fclose(fp);
    fclose(fp_err);
    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // Translates virtual-key messages into character messages. The character messages are posted to the calling thread's message queue, to be read the next time the thread calls the GetMessage or PeekMessage function
        DispatchMessage(&msg); // Dispatches a message to a window procedure. It is typically used to dispatch a message retrieved by the GetMessage function.
    }

    return 0;
}
#elif __linux__
int main() {

}
#elif __APPLE__
#endif