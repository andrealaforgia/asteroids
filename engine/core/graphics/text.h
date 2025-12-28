#ifndef TEXT_H_
#define TEXT_H_

#include <SDL.h>

#include "geometry.h"
#include "graphics.h"

typedef struct {
  int width;
  int height;
} text_dimensions_t;

text_dimensions_t calculate_text_dimensions(const char *s, int scale);

point_t write_text(const graphics_context_ptr graphics_context, const char *s,
                   const point_t position, int scale, color_t color);

point_t write_number(const graphics_context_ptr graphics_context,
                     const point_t position, int number, int scale);

#endif  // TEXT_H_
