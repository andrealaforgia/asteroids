#include <stdlib.h>
#include <string.h>

#include "renderer.h"

#include "color.h"

/**
 * Mock renderer implementation for testing
 *
 * This renderer doesn't actually draw anything but tracks draw calls
 * and maintains a simple pixel buffer for verification.
 */

typedef struct {
  int width;
  int height;
  color_t* pixel_buffer;  // Simple framebuffer for testing
  int draw_call_count;    // Statistics for testing
  int pixel_count;
  int line_count;
  int circle_count;
  int polygon_count;
} mock_renderer_impl_t;

// Mock renderer vtable implementations

static void mock_renderer_destroy(renderer_t* renderer) {
  if (renderer) {
    mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;
    if (impl) {
      free(impl->pixel_buffer);
      free(impl);
    }
    free(renderer);
  }
}

static void mock_renderer_clear(renderer_t* renderer) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;
  memset(impl->pixel_buffer, 0,
         impl->width * impl->height * sizeof(color_t));
}

static void mock_renderer_present(renderer_t* renderer) {
  // No-op for mock renderer
  (void)renderer;
}

static void mock_renderer_draw_pixel(renderer_t* renderer, int x, int y,
                                     color_t color) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;

  impl->pixel_count++;
  impl->draw_call_count++;

  // Bounds check
  if (x >= 0 && x < impl->width && y >= 0 && y < impl->height) {
    impl->pixel_buffer[y * impl->width + x] = color;
  }
}

static void mock_renderer_draw_line(renderer_t* renderer, int x1, int y1,
                                    int x2, int y2, color_t color) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;

  impl->line_count++;
  impl->draw_call_count++;

  // Simple Bresenham line algorithm for pixel buffer
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    mock_renderer_draw_pixel(renderer, x1, y1, color);

    if (x1 == x2 && y1 == y2) break;

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

static void mock_renderer_draw_circle(renderer_t* renderer, int32_t center_x,
                                      int32_t center_y, int32_t radius,
                                      color_t color) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;

  impl->circle_count++;
  impl->draw_call_count++;

  // Simple circle approximation (not drawing actual pixels to keep it fast)
  (void)center_x;
  (void)center_y;
  (void)radius;
  (void)color;
}

static void mock_renderer_draw_filled_polygon(renderer_t* renderer,
                                              const struct SDL_Point* points,
                                              int num_points, color_t color) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;

  impl->polygon_count++;
  impl->draw_call_count++;

  // Don't actually fill for mock (expensive and not needed for tests)
  (void)points;
  (void)num_points;
  (void)color;
}

static int mock_renderer_get_width(renderer_t* renderer) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;
  return impl->width;
}

static int mock_renderer_get_height(renderer_t* renderer) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;
  return impl->height;
}

// Factory functions

renderer_ptr create_mock_renderer(int width, int height) {
  renderer_ptr renderer = malloc(sizeof(renderer_t));
  if (!renderer) return NULL;

  mock_renderer_impl_t* impl = malloc(sizeof(mock_renderer_impl_t));
  if (!impl) {
    free(renderer);
    return NULL;
  }

  impl->width = width;
  impl->height = height;
  impl->pixel_buffer = calloc(width * height, sizeof(color_t));
  impl->draw_call_count = 0;
  impl->pixel_count = 0;
  impl->line_count = 0;
  impl->circle_count = 0;
  impl->polygon_count = 0;

  if (!impl->pixel_buffer) {
    free(impl);
    free(renderer);
    return NULL;
  }

  renderer->impl = impl;
  renderer->destroy = mock_renderer_destroy;
  renderer->clear = mock_renderer_clear;
  renderer->present = mock_renderer_present;
  renderer->draw_pixel = mock_renderer_draw_pixel;
  renderer->draw_line = mock_renderer_draw_line;
  renderer->draw_circle = mock_renderer_draw_circle;
  renderer->draw_filled_polygon = mock_renderer_draw_filled_polygon;
  renderer->get_width = mock_renderer_get_width;
  renderer->get_height = mock_renderer_get_height;

  return renderer;
}

// Headless renderer is just a mock renderer alias
renderer_ptr create_headless_renderer(int width, int height) {
  return create_mock_renderer(width, height);
}

// Testing utilities (optional, for unit tests)

color_t mock_renderer_get_pixel(renderer_ptr renderer, int x, int y) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;
  if (x >= 0 && x < impl->width && y >= 0 && y < impl->height) {
    return impl->pixel_buffer[y * impl->width + x];
  }
  return 0;
}

int mock_renderer_get_draw_call_count(renderer_ptr renderer) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;
  return impl->draw_call_count;
}

void mock_renderer_reset_stats(renderer_ptr renderer) {
  mock_renderer_impl_t* impl = (mock_renderer_impl_t*)renderer->impl;
  impl->draw_call_count = 0;
  impl->pixel_count = 0;
  impl->line_count = 0;
  impl->circle_count = 0;
  impl->polygon_count = 0;
}
