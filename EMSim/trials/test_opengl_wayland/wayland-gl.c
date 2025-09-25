#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-cursor.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "xdg-shell-client-protocol.h"

// Globals
struct wl_display *display;
struct wl_compositor *compositor;
struct wl_surface *surface;
struct wl_egl_window *egl_window;
struct xdg_wm_base *xdg_wm_base;
struct xdg_surface *xdg_surface;
struct xdg_toplevel *xdg_toplevel;

EGLDisplay egl_display;
EGLConfig egl_config;
EGLContext egl_context;
EGLSurface egl_surface;

int running = 1;

// xdg_wm_base listener
static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

// xdg_surface listener
static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    xdg_surface_ack_configure(xdg_surface, serial);
    // Render the scene after the surface is configured
    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red color
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(egl_display, egl_surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

// xdg_toplevel listener
static void xdg_toplevel_configure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    // We don't handle resizing in this simple example, but you would
    // update the egl_window size here if you did.
    if (width > 0 && height > 0) {
        wl_egl_window_resize(egl_window, width, height, 0, 0);
    }
}

static void xdg_toplevel_close(void *data, struct xdg_toplevel *xdg_toplevel) {
    running = 0;
}

static const struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close,
};

// Global registry listener
static void registry_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, "xdg_wm_base") == 0) {
        xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(xdg_wm_base, &xdg_wm_base_listener, NULL);
    }
}

static void registry_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
    // This space intentionally left blank.
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

int main() {
    // 1. Connect to the Wayland display
    display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display.\n");
        return -1;
    }

    // 2. Get Wayland globals
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    if (!compositor || !xdg_wm_base) {
        fprintf(stderr, "Failed to get compositor or xdg_wm_base.\n");
        return -1;
    }

    // 3. Initialize EGL
    egl_display = eglGetDisplay(display);
    if (egl_display == EGL_NO_DISPLAY) {
        fprintf(stderr, "Failed to get EGL display.\n");
        return -1;
    }
    eglInitialize(egl_display, NULL, NULL);
    
    // 4. Choose EGL config
    EGLint attributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLint num_configs;
    eglChooseConfig(egl_display, attributes, &egl_config, 1, &num_configs);

    // 5. Create EGL context
    EGLint context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attributes);
    if (egl_context == EGL_NO_CONTEXT) {
        fprintf(stderr, "Failed to create EGL context.\n");
        return -1;
    }

    // 6. Create Wayland surface and xdg_surface
    surface = wl_compositor_create_surface(compositor);
    xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface);
    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, NULL);

    xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);
    xdg_toplevel_add_listener(xdg_toplevel, &xdg_toplevel_listener, NULL);
    xdg_toplevel_set_title(xdg_toplevel, "Wayland xdg-shell OpenGL Example");

    wl_surface_commit(surface); // Commit the changes

    // 7. Create EGL window and surface
    egl_window = wl_egl_window_create(surface, 640, 480);
    egl_surface = eglCreateWindowSurface(egl_display, egl_config, egl_window, NULL);
    if (egl_surface == EGL_NO_SURFACE) {
        fprintf(stderr, "Failed to create EGL surface.\n");
        return -1;
    }

    // Main event loop
    while (running) {
        wl_display_dispatch_pending(display);
    }

    // 8. Cleanup
    eglDestroySurface(egl_display, egl_surface);
    eglDestroyContext(egl_display, egl_context);
    wl_egl_window_destroy(egl_window);
    xdg_toplevel_destroy(xdg_toplevel);
    xdg_surface_destroy(xdg_surface);
    wl_surface_destroy(surface);
    wl_compositor_destroy(compositor);
    xdg_wm_base_destroy(xdg_wm_base);
    wl_registry_destroy(registry);
    eglTerminate(egl_display);
    wl_display_disconnect(display);

    return 0;
}