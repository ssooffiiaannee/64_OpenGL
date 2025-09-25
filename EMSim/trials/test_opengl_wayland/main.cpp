#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xdg-shell-client-protocol.h"

// Structure to hold Wayland globals
struct wl_globals {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_surface *surface;

    struct xdg_wm_base *xdg_wm_base;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;

    EGLDisplay egl_display;
    EGLConfig egl_config;
    EGLContext egl_context;
    EGLSurface egl_surface;
};

static struct wl_globals globals;

// --- XDG Shell handlers ---

static void xdg_wm_base_ping(void *data,
                             struct xdg_wm_base *xdg_wm_base,
                             uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

// --- Wayland Event Handlers ---

static void registry_handle_global(void *data, struct wl_registry *registry,
                                   uint32_t name, const char *interface,
                                   uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
        globals.compositor =  (wl_compositor*) wl_registry_bind(registry, name,
                                              &wl_compositor_interface, 1);
    } else if (strcmp(interface, "xdg_wm_base") == 0) {
        globals.xdg_wm_base = (xdg_wm_base*) wl_registry_bind(registry, name,
                                               &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(globals.xdg_wm_base,
                                 &xdg_wm_base_listener, NULL);
    }
}

static void registry_handle_global_remove(void *data,
                                          struct wl_registry *registry,
                                          uint32_t name) {
    // Not needed in this simple example
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handle_global,
    .global_remove = registry_handle_global_remove,
};

// --- EGL Initialization ---

static EGLBoolean create_egl_context(struct wl_surface *surface) {
    globals.egl_display = eglGetDisplay(globals.display);
    if (globals.egl_display == EGL_NO_DISPLAY) {
        fprintf(stderr, "Failed to get EGL display\n");
        return EGL_FALSE;
    }

    EGLint major, minor;
    if (!eglInitialize(globals.egl_display, &major, &minor)) {
        fprintf(stderr, "Failed to initialize EGL\n");
        return EGL_FALSE;
    }

    EGLint num_configs;
    EGLint config_attribs[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 0,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    if (!eglChooseConfig(globals.egl_display, config_attribs,
                         &globals.egl_config, 1, &num_configs) ||
        num_configs == 0) {
        fprintf(stderr, "Failed to choose EGL config\n");
        return EGL_FALSE;
    }

    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    globals.egl_context = eglCreateContext(globals.egl_display,
                                           globals.egl_config,
                                           EGL_NO_CONTEXT,
                                           context_attribs);
    if (globals.egl_context == EGL_NO_CONTEXT) {
        fprintf(stderr, "Failed to create EGL context\n");
        return EGL_FALSE;
    }

    struct wl_egl_window *egl_window =
        wl_egl_window_create(surface, 640, 480);
    if (!egl_window) {
        fprintf(stderr, "Failed to create EGL window\n");
        return EGL_FALSE;
    }

    globals.egl_surface =
        eglCreateWindowSurface(globals.egl_display,
                               globals.egl_config,
                               egl_window, NULL);
    if (globals.egl_surface == EGL_NO_SURFACE) {
        fprintf(stderr, "Failed to create EGL window surface\n");
        return EGL_FALSE;
    }

    if (!eglMakeCurrent(globals.egl_display,
                        globals.egl_surface,
                        globals.egl_surface,
                        globals.egl_context)) {
        fprintf(stderr, "Failed to make EGL context current\n");
        return EGL_FALSE;
    }

    return EGL_TRUE;
}

// --- OpenGL Drawing ---

static void draw_frame() {
    glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(globals.egl_display, globals.egl_surface);
}

// --- Main Function ---

int main() {
    globals.display = wl_display_connect(NULL);
    if (!globals.display) {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        return 1;
    }

    globals.registry = wl_display_get_registry(globals.display);
    wl_registry_add_listener(globals.registry, &registry_listener, NULL);

    // Round trip to fetch globals
    wl_display_roundtrip(globals.display);

    if (!globals.compositor) {
        fprintf(stderr, "Compositor not available\n");
        return 1;
    }

    if (!globals.xdg_wm_base) {
        fprintf(stderr, "xdg_wm_base not available\n");
        return 1;
    }

    globals.surface = wl_compositor_create_surface(globals.compositor);
    if (!globals.surface) {
        fprintf(stderr, "Failed to create Wayland surface\n");
        return 1;
    }

    globals.xdg_surface =
        xdg_wm_base_get_xdg_surface(globals.xdg_wm_base, globals.surface);
    globals.xdg_toplevel =
        xdg_surface_get_toplevel(globals.xdg_surface);

    xdg_toplevel_set_title(globals.xdg_toplevel, "Wayland XDG Example");
    wl_surface_commit(globals.surface);

    if (!create_egl_context(globals.surface)) {
        return 1;
    }

    printf("Wayland/OpenGL XDG example running. Close window to exit.\n");

    // Main event loop
    while (1) {
        wl_display_dispatch_pending(globals.display);
        draw_frame();
    }

    // Cleanup (never reached in this demo)
    eglDestroySurface(globals.egl_display, globals.egl_surface);
    eglDestroyContext(globals.egl_display, globals.egl_context);
    xdg_toplevel_destroy(globals.xdg_toplevel);
    xdg_surface_destroy(globals.xdg_surface);
    xdg_wm_base_destroy(globals.xdg_wm_base);
    wl_surface_destroy(globals.surface);
    if (globals.compositor) wl_compositor_destroy(globals.compositor);
    wl_registry_destroy(globals.registry);
    wl_display_disconnect(globals.display);

    return 0;
}
