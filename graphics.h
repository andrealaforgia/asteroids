#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SDL.h>
#include <stdbool.h>

#include "color.h"
#include "geometry.h"
#include "window_mode.h"

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int screen_width;
  int screen_height;
  point_t screen_center;
} graphics_context_t, *graphics_context_ptr;

void print_graphics_info(void);

graphics_context_t init_graphics_context(int display, int display_mode,
                                         window_mode_t window_mode);
void terminate_graphics_context(const graphics_context_ptr graphics_context);
void draw_line_between_points(const graphics_context_ptr graphics_context,
                              const point_ptr p1, const point_ptr p2,
                              color_t color);
void draw_line(const graphics_context_ptr graphics_context, int x1, int y1,
               int x2, int y2, color_t color);
void draw_pixel(const graphics_context_ptr graphics_context, int x, int y,
                color_t color);
void draw_point(const graphics_context_ptr graphics_context, const point_ptr p,
                color_t color);
void draw_fat_pixel(const graphics_context_ptr graphics_context,
                    const point_ptr p, color_t color);
void draw_circle(const graphics_context_ptr graphics_context, int32_t centreX,
                 int32_t centreY, int32_t radius, color_t color);
double wrap_x(const graphics_context_ptr graphics_context, double x);
double wrap_y(const graphics_context_ptr graphics_context, double y);
point_t random_point(const graphics_context_ptr graphics_context);
color_t random_color(void);
bool out_of_bounds(const graphics_context_ptr graphics_context,
                   const point_ptr point);
point_t random_point_around(const point_ptr point, int min_tolerance,
                            int max_tolerance);

#endif  // GRAPHICS_H_
