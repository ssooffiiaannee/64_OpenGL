// g++ x11_window.c -o prog -lX11

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
// g++ x11_window.c -o prog -lX11
int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;

    // Connect to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(display);

    // Create a window
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 800, 600, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    // Select input events to listen for (like key press, expose, etc.)
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // Map (show) the window
    XMapWindow(display, window);

    // Event loop
    while (1) {
        XNextEvent(display, &event);

        // Handle expose event (redraw window content)
        if (event.type == Expose) {
            XFillRectangle(display, window, DefaultGC(display, screen), 20, 20, 10, 10);
        }

        // Handle key press event
        if (event.type == KeyPress) {
            break;  // Exit the loop on key press
        }
    }

    // Close the display connection
    XCloseDisplay(display);
    return 0;
}
