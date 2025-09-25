#include <wayland-client.h>
#include <wayland-client-protocol.h>
// #include <wayland-protocols/stable/xdg-shell.h>
#include "xdg-shell-client-protocol.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 300
#define HEIGHT 200

struct wl_compositor *compositor = NULL;
struct wl_shm *shm = NULL;
struct xdg_wm_base *xdg_wm_base = NULL;
struct wl_surface *surface = NULL;
struct xdg_surface *xdg_surface = NULL;
struct xdg_toplevel *xdg_toplevel = NULL;
struct wl_buffer *buffer;
void *shm_data;

// Creates a shared memory buffer
static struct wl_buffer *create_shm_buffer(struct wl_shm *shm, int width, int height) {
    int stride = width * 4;
    int size = stride * height;
    int fd = shm_open("/wayland-shm", O_RDWR | O_CREAT | O_EXCL, 0600);
    shm_unlink("/wayland-shm");

    if (fd < 0) {
        perror("shm_open failed");
        return NULL;
    }
    if (ftruncate(fd, size) < 0) {
        perror("ftruncate failed");
        close(fd);
        return NULL;
    }

    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    shm_data = data;
    return buffer;
}

// Fills the buffer with a solid color
static void draw() {
    uint32_t *pixel = shm_data;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        *pixel++ = 0xFF0000FF;  // Blue color (XRGB8888 format)
    }
}

// Handles Wayland registry events
static void registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    } else if (strcmp(interface, "wl_shm") == 0) {
        shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, "xdg_wm_base") == 0) {
        xdg_wm_base = wl_registry_bind(registry, id, &xdg_wm_base_interface, 1);
    }
}

static void registry_remover(void *data, struct wl_registry *registry, uint32_t id) {}

// Callback when xdg_surface is configured
static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    xdg_surface_ack_configure(xdg_surface, serial);
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);
}

// Main function
int main() {
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        return -1;
    }

    struct wl_registry *registry = wl_display_get_registry(display);
    struct wl_registry_listener registry_listener = {
        .global = registry_handler,
        .global_remove = registry_remover,
    };
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);

    if (!compositor || !shm || !xdg_wm_base) {
        fprintf(stderr, "Missing required Wayland interfaces\n");
        return -1;
    }

    // Create surface
    surface = wl_compositor_create_surface(compositor);

    // Create XDG shell surface
    xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface);
    xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);
    xdg_surface_add_listener(xdg_surface, &(struct xdg_surface_listener){ .configure = xdg_surface_configure }, NULL);

    wl_surface_commit(surface);

    // Create shared memory buffer
    buffer = create_shm_buffer(shm, WIDTH, HEIGHT);
    if (!buffer) {
        fprintf(stderr, "Failed to create buffer\n");
        return -1;
    }

    // Draw something
    draw();

    printf("Window should now be visible!\n");

    // Run event loop
    while (wl_display_dispatch(display) != -1) {}

    wl_surface_destroy(surface);
    wl_display_disconnect(display);
    return 0;
}
