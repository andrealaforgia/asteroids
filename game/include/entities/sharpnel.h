#ifndef SHARPNEL_H_
#define SHARPNEL_H_

#include <stdbool.h>
#include <stdlib.h>

#include "geometry.h"
#include "graphics.h"
#include "inline.h"

typedef struct {
  point_t position;
  double scale;
  int creation_ticks;
  bool active;
} sharpnel_t, *sharpnel_ptr;

sharpnel_t create_sharpnel(point_t position);
void add_sharpnel(point_t position);
void remove_sharpnel(size_t sharpnel_index);
void render_sharpnel(const graphics_context_ptr graphics_context,
                     const sharpnel_ptr sharpnel, color_t color);
void update_sharpnel(const graphics_context_ptr graphics_context,
                     size_t sharpnel_index);
void animate_sharpnels(const graphics_context_ptr graphics_context);
void reset_sharpnels(void);
#endif  // SHARPNEL_H_
