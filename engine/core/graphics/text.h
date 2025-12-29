/**
 * @file text.h
 * @brief Bitmap font text rendering system
 *
 * Provides functions for rendering text and numbers using a custom
 * bitmap font. Supports scalable text rendering and calculates text
 * dimensions for layout purposes.
 */

#ifndef ENGINE_CORE_GRAPHICS_TEXT_H_
#define ENGINE_CORE_GRAPHICS_TEXT_H_

#include <SDL.h>

#include "geometry.h"
#include "graphics.h"

typedef struct {
  int width;
  int height;
} text_dimensions_t;

text_dimensions_t calculate_text_dimensions(const char* s, int scale);

point_t write_text(const graphics_context_ptr graphics_context, const char* s,
                   const point_t position, int scale, color_t color);

point_t write_number(const graphics_context_ptr graphics_context,
                     const point_t position, int number, int scale);

#endif  // ENGINE_CORE_GRAPHICS_TEXT_H_
