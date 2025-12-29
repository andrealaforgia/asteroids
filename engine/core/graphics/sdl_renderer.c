#include <SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "renderer.h"

#include "color.h"

// SDL renderer implementation data
typedef struct {
  SDL_Renderer* sdl_renderer;
  int width;
  int height;
} sdl_renderer_impl_t;

// Pre-calculated circle lookup table
#define CIRCLE_POINTS 360
static double circle_cos[CIRCLE_POINTS];
static double circle_sin[CIRCLE_POINTS];
static bool circle_lookup_initialized = false;

static void init_circle_lookup(void) {
  if (!circle_lookup_initialized) {
    for (int i = 0; i < CIRCLE_POINTS; i++) {
      double angle = i * M_PI / 180.0;
      circle_cos[i] = cos(angle);
      circle_sin[i] = sin(angle);
    }
    circle_lookup_initialized = true;
  }
}

// SDL renderer vtable implementations

static void sdl_renderer_destroy(renderer_t* renderer) {
  if (renderer) {
    free(renderer->impl);
    free(renderer);
  }
}

static void sdl_renderer_clear(renderer_t* renderer) {
  sdl_renderer_impl_t* impl = (sdl_renderer_impl_t*)renderer->impl;
  SDL_SetRenderDrawColor(impl->sdl_renderer, 0, 0, 0, 255);
  SDL_RenderClear(impl->sdl_renderer);
}

static void sdl_renderer_present(renderer_t* renderer) {
  sdl_renderer_impl_t* impl = (sdl_renderer_impl_t*)renderer->impl;
  SDL_RenderPresent(impl->sdl_renderer);
}

static void sdl_renderer_draw_pixel(renderer_t* renderer, int x, int y,
                                    color_t color) {
  sdl_renderer_impl_t* impl = (sdl_renderer_impl_t*)renderer->impl;
  SDL_SetRenderDrawColor(impl->sdl_renderer, R(color), G(color), B(color), 255);
  SDL_RenderDrawPoint(impl->sdl_renderer, x, y);
}

static void sdl_renderer_draw_line(renderer_t* renderer, int x1, int y1,
                                   int x2, int y2, color_t color) {
  sdl_renderer_impl_t* impl = (sdl_renderer_impl_t*)renderer->impl;
  SDL_SetRenderDrawColor(impl->sdl_renderer, R(color), G(color), B(color), 255);
  SDL_RenderDrawLine(impl->sdl_renderer, x1, y1, x2, y2);
}

static void sdl_renderer_draw_circle(renderer_t* renderer, int32_t center_x,
                                     int32_t center_y, int32_t radius,
                                     color_t color) {
  sdl_renderer_impl_t* impl = (sdl_renderer_impl_t*)renderer->impl;

  SDL_Point points[CIRCLE_POINTS];
  for (int i = 0; i < CIRCLE_POINTS; i++) {
    points[i].x = (int)(center_x + radius * circle_cos[i]);
    points[i].y = (int)(center_y + radius * circle_sin[i]);
  }

  SDL_SetRenderDrawColor(impl->sdl_renderer, R(color), G(color), B(color), 255);
  SDL_RenderDrawPoints(impl->sdl_renderer, points, CIRCLE_POINTS);
}

static void sdl_renderer_draw_filled_polygon(renderer_t* renderer,
                                             const SDL_Point* points,
                                             int num_points, color_t color) {
  sdl_renderer_impl_t* impl = (sdl_renderer_impl_t*)renderer->impl;

  if (num_points < 3) return;

  SDL_SetRenderDrawColor(impl->sdl_renderer, R(color), G(color), B(color), 255);

  // Find bounding box
  int min_x = points[0].x, max_x = points[0].x;
  int min_y = points[0].y, max_y = points[0].y;
  for (int i = 1; i < num_points; i++) {
    if (points[i].x < min_x) min_x = points[i].x;
    if (points[i].x > max_x) max_x = points[i].x;
    if (points[i].y < min_y) min_y = points[i].y;
    if (points[i].y > max_y) max_y = points[i].y;
  }

  // Scan line fill
  for (int y = min_y; y <= max_y; y++) {
    int intersections[64];
    int intersection_count = 0;

    for (int i = 0; i < num_points; i++) {
      int next = (i + 1) % num_points;
      int y1 = points[i].y;
      int y2 = points[next].y;

      if ((y1 <= y && y < y2) || (y2 <= y && y < y1)) {
        int x1 = points[i].x;
        int x2 = points[next].x;
        int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
        if (intersection_count < 64) {
          intersections[intersection_count++] = x;
        }
      }
    }

    // Sort intersections
    for (int i = 0; i < intersection_count - 1; i++) {
      for (int j = i + 1; j < intersection_count; j++) {
        if (intersections[i] > intersections[j]) {
          int temp = intersections[i];
          intersections[i] = intersections[j];
          intersections[j] = temp;
        }
      }
    }

    // Draw horizontal lines between pairs
    for (int i = 0; i < intersection_count; i += 2) {
      if (i + 1 < intersection_count) {
        SDL_RenderDrawLine(impl->sdl_renderer, intersections[i], y,
                          intersections[i + 1], y);
      }
    }
  }
}

static int sdl_renderer_get_width(renderer_t* renderer) {
  sdl_renderer_impl_t* impl = (sdl_renderer_impl_t*)renderer->impl;
  return impl->width;
}

static int sdl_renderer_get_height(renderer_t* renderer) {
  sdl_renderer_impl_t* impl = (sdl_renderer_impl_t*)renderer->impl;
  return impl->height;
}

// Factory function
renderer_ptr create_sdl_renderer(void* sdl_renderer, int width, int height) {
  init_circle_lookup();

  renderer_ptr renderer = malloc(sizeof(renderer_t));
  if (!renderer) return NULL;

  sdl_renderer_impl_t* impl = malloc(sizeof(sdl_renderer_impl_t));
  if (!impl) {
    free(renderer);
    return NULL;
  }

  impl->sdl_renderer = (SDL_Renderer*)sdl_renderer;
  impl->width = width;
  impl->height = height;

  renderer->impl = impl;
  renderer->destroy = sdl_renderer_destroy;
  renderer->clear = sdl_renderer_clear;
  renderer->present = sdl_renderer_present;
  renderer->draw_pixel = sdl_renderer_draw_pixel;
  renderer->draw_line = sdl_renderer_draw_line;
  renderer->draw_circle = sdl_renderer_draw_circle;
  renderer->draw_filled_polygon = sdl_renderer_draw_filled_polygon;
  renderer->get_width = sdl_renderer_get_width;
  renderer->get_height = sdl_renderer_get_height;

  return renderer;
}
