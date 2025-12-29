#ifndef ENGINE_CORE_GRAPHICS_RENDERER_H_
#define ENGINE_CORE_GRAPHICS_RENDERER_H_

#include <stdbool.h>
#include <stdint.h>

#include "color.h"
#include "geometry.h"

/**
 * Abstract renderer interface
 *
 * This interface decouples rendering logic from SDL2 implementation,
 * enabling:
 * - Headless testing without SDL initialization
 * - Alternative rendering backends (OpenGL, Vulkan, software)
 * - Mock renderers for unit tests
 */

// Forward declare SDL types for SDL implementation
struct SDL_Point;

// Renderer interface - vtable pattern in C
typedef struct renderer_t {
  void* impl;  // Implementation-specific data (SDL_Renderer*, etc.)

  // Lifecycle
  void (*destroy)(struct renderer_t* renderer);

  // Frame management
  void (*clear)(struct renderer_t* renderer);
  void (*present)(struct renderer_t* renderer);

  // Drawing primitives
  void (*draw_pixel)(struct renderer_t* renderer, int x, int y, color_t color);
  void (*draw_line)(struct renderer_t* renderer, int x1, int y1, int x2, int y2,
                    color_t color);
  void (*draw_circle)(struct renderer_t* renderer, int32_t center_x,
                      int32_t center_y, int32_t radius, color_t color);
  void (*draw_filled_polygon)(struct renderer_t* renderer,
                              const struct SDL_Point* points, int num_points,
                              color_t color);

  // Renderer capabilities/info
  int (*get_width)(struct renderer_t* renderer);
  int (*get_height)(struct renderer_t* renderer);
} renderer_t;

typedef renderer_t* renderer_ptr;

// Factory functions for different renderer implementations
renderer_ptr create_sdl_renderer(void* sdl_renderer, int width, int height);
renderer_ptr create_mock_renderer(int width, int height);
renderer_ptr create_headless_renderer(int width, int height);

// Common renderer operations (calls through vtable)
static inline void renderer_destroy(renderer_ptr renderer) {
  if (renderer && renderer->destroy) {
    renderer->destroy(renderer);
  }
}

static inline void renderer_clear(renderer_ptr renderer) {
  if (renderer && renderer->clear) {
    renderer->clear(renderer);
  }
}

static inline void renderer_present(renderer_ptr renderer) {
  if (renderer && renderer->present) {
    renderer->present(renderer);
  }
}

static inline void renderer_draw_pixel(renderer_ptr renderer, int x, int y,
                                       color_t color) {
  if (renderer && renderer->draw_pixel) {
    renderer->draw_pixel(renderer, x, y, color);
  }
}

static inline void renderer_draw_line(renderer_ptr renderer, int x1, int y1,
                                      int x2, int y2, color_t color) {
  if (renderer && renderer->draw_line) {
    renderer->draw_line(renderer, x1, y1, x2, y2, color);
  }
}

static inline void renderer_draw_circle(renderer_ptr renderer, int32_t center_x,
                                        int32_t center_y, int32_t radius,
                                        color_t color) {
  if (renderer && renderer->draw_circle) {
    renderer->draw_circle(renderer, center_x, center_y, radius, color);
  }
}

static inline void renderer_draw_filled_polygon(renderer_ptr renderer,
                                                const struct SDL_Point* points,
                                                int num_points, color_t color) {
  if (renderer && renderer->draw_filled_polygon) {
    renderer->draw_filled_polygon(renderer, points, num_points, color);
  }
}

static inline int renderer_get_width(renderer_ptr renderer) {
  if (renderer && renderer->get_width) {
    return renderer->get_width(renderer);
  }
  return 0;
}

static inline int renderer_get_height(renderer_ptr renderer) {
  if (renderer && renderer->get_height) {
    return renderer->get_height(renderer);
  }
  return 0;
}

#endif  // ENGINE_CORE_GRAPHICS_RENDERER_H_
