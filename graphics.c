#include "graphics.h"

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "inline.h"

static int get_display_count(void) { return SDL_GetNumVideoDisplays(); }

static SDL_DisplayMode *get_display_modes(int display_index,
                                          int *p_display_mode_count) {
  *p_display_mode_count = SDL_GetNumDisplayModes(display_index);
  if (*p_display_mode_count < 1) {
    SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
    return NULL;
  }

  SDL_DisplayMode *display_modes =
      calloc(*p_display_mode_count, sizeof(SDL_DisplayMode));

  for (int i = 0; i < *p_display_mode_count; i++) {
    if (SDL_GetDisplayMode(display_index, i, &display_modes[i]) != 0) {
      SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
      free(display_modes);
      return NULL;
    }
  }
  return display_modes;
}

void print_graphics_info(void) {
  SDL_Init(SDL_INIT_EVERYTHING);
  int display_count = get_display_count();
  SDL_Log("Number of available displays: %d\n", display_count);
  for (int display_index = 0; display_index < display_count; display_index++) {
    SDL_Log("Display Index: %d\n", display_index);
    int display_mode_count;
    SDL_DisplayMode *display_modes =
        get_display_modes(display_index, &display_mode_count);
    if (display_modes) {
      for (int dm = 0; dm < display_mode_count; dm++) {
        SDL_Log("Display Mode %d\tbpp %d\t%s\t%d x %d", dm,
                SDL_BITSPERPIXEL(display_modes[dm].format),
                SDL_GetPixelFormatName(display_modes[dm].format),
                display_modes[dm].w, display_modes[dm].h);
      }
      free(display_modes);
    }
  }
  SDL_Quit();
}

graphics_context_t init_graphics_context(int display, int display_mode,
                                         window_mode_t window_mode) {
  graphics_context_t graphics_context = {0};

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("SDL Init Error: %s\n", SDL_GetError());
    abort();
  }

  SDL_ShowCursor(SDL_DISABLE);
  SDL_DisplayMode sdl_display_mode;

  if (SDL_GetDisplayMode(display, display_mode, &sdl_display_mode) != 0) {
    SDL_Log("SDL_GetDisplayMode Error: %s\n", SDL_GetError());
    abort();
  }

  SDL_Log("Display Mode: w=%d h=%d refresh=%d\n", sdl_display_mode.w,
          sdl_display_mode.h, sdl_display_mode.refresh_rate);

  graphics_context.screen_width = sdl_display_mode.w;
  graphics_context.screen_height = sdl_display_mode.h;
  graphics_context.screen_center = point(graphics_context.screen_width / 2,
                                         graphics_context.screen_height / 2);

  Uint32 window_flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

  if (window_mode == FULL_SCREEN) {
    window_flags |= SDL_WINDOW_FULLSCREEN;
  } else {
    window_flags |= SDL_WINDOW_MAXIMIZED;
  }

  graphics_context.window = SDL_CreateWindow(
      "Asteroids", SDL_WINDOWPOS_CENTERED_DISPLAY(display),
      SDL_WINDOWPOS_CENTERED_DISPLAY(display), graphics_context.screen_width,
      graphics_context.screen_height, window_flags);

  if (!graphics_context.window) {
    SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
    abort();
  }

  if (window_mode == FULL_SCREEN) {
    if (SDL_SetWindowDisplayMode(graphics_context.window, &sdl_display_mode) !=
        0) {
      SDL_Log("SDL_SetWindowDisplayMode Error: %s\n", SDL_GetError());
      abort();
    }
    SDL_SetWindowFullscreen(graphics_context.window, SDL_WINDOW_FULLSCREEN);
  }

  int drawable_w, drawable_h;
  SDL_GL_GetDrawableSize(graphics_context.window, &drawable_w, &drawable_h);
  SDL_Log("Drawable Size: w=%d h=%d\n", drawable_w, drawable_h);

  graphics_context.renderer =
      SDL_CreateRenderer(graphics_context.window, -1, SDL_RENDERER_ACCELERATED);
  if (!graphics_context.renderer) {
    SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    abort();
  }

  SDL_RenderSetLogicalSize(graphics_context.renderer,
                           graphics_context.screen_width,
                           graphics_context.screen_height);

  return graphics_context;
}

void terminate_graphics_context(const graphics_context_ptr graphics_context) {
  SDL_DestroyRenderer(graphics_context->renderer);
  SDL_DestroyWindow(graphics_context->window);
  SDL_Quit();
}

ALWAYS_INLINE void draw_line(const graphics_context_ptr graphics_context,
                             int x1, int y1, int x2, int y2, color_t color) {
  SDL_SetRenderDrawColor(graphics_context->renderer, R(color), G(color),
                         B(color), 255);
  SDL_RenderDrawLine(graphics_context->renderer, x1, y1, x2, y2);
}

ALWAYS_INLINE void draw_line_between_points(
    const graphics_context_ptr graphics_context, const point_ptr p1,
    const point_ptr p2, color_t color) {
  draw_line(graphics_context, p1->x, p1->y, p2->x, p2->y, color);
}

ALWAYS_INLINE void draw_pixel(const graphics_context_ptr graphics_context,
                              int x, int y, color_t color) {
  SDL_SetRenderDrawColor(graphics_context->renderer, R(color), G(color),
                         B(color), 255);
  SDL_RenderDrawPoint(graphics_context->renderer, x, y);
}

ALWAYS_INLINE void draw_point(const graphics_context_ptr graphics_context,
                              const point_ptr p, color_t color) {
  draw_pixel(graphics_context, p->x, p->y, color);
}

ALWAYS_INLINE void draw_fat_pixel(const graphics_context_ptr graphics_context,
                                  const point_ptr p, color_t color) {
  SDL_SetRenderDrawColor(graphics_context->renderer, R(color), G(color),
                         B(color), 255);
  SDL_RenderDrawLine(graphics_context->renderer, p->x - 1, p->y - 1, p->x + 1,
                     p->y - 1);
  SDL_RenderDrawLine(graphics_context->renderer, p->x - 1, p->y, p->x + 1,
                     p->y);
  SDL_RenderDrawLine(graphics_context->renderer, p->x - 1, p->y + 1, p->x + 1,
                     p->y + 1);
}

void draw_circle(const graphics_context_ptr graphics_context, int32_t centreX,
                 int32_t centreY, int32_t radius, color_t color) {
  SDL_SetRenderDrawColor(graphics_context->renderer, R(color), G(color),
                         B(color), 255);
  for (int i = 0; i < 360; i++) {
    double angle = i * M_PI / 180;
    int x = centreX + (int)(radius * cos(angle));
    int y = centreY + (int)(radius * sin(angle));
    draw_pixel(graphics_context, x, y, color);
  }
}

ALWAYS_INLINE double wrap(double value, double upper_bound) {
  if (value < 0) {
    return upper_bound - 1;
  }
  if (value > upper_bound - 1) {
    return 0;
  }
  return value;
}

ALWAYS_INLINE double wrap_x(const graphics_context_ptr graphics_context,
                            double x) {
  return wrap(x, graphics_context->screen_width);
}

ALWAYS_INLINE double wrap_y(const graphics_context_ptr graphics_context,
                            double y) {
  return wrap(y, graphics_context->screen_height);
}

ALWAYS_INLINE point_t
random_point(const graphics_context_ptr graphics_context) {
  return point(rand() % graphics_context->screen_width,
               rand() % graphics_context->screen_height);
}

ALWAYS_INLINE color_t random_color(void) {
  int r = 64;
  int g = 96 + rand() % 128;
  int b = 128 + rand() % 128;
  return COLOR(r, g, b);
}

ALWAYS_INLINE bool out_of_bounds(const graphics_context_ptr graphics_context,
                                 const point_ptr point) {
  return point->x < 0 || point->x >= graphics_context->screen_width ||
         point->y < 0 || point->y >= graphics_context->screen_height;
}
