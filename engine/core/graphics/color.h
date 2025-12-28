#ifndef COLOR_H_
#define COLOR_H_

#include "inline.h"

#define COLOR_BLACK 0x000000
#define COLOR_WHITE 0xFFFFFF
#define COLOR_YELLOW 0xFFFF00
#define COLOR_DARK_YELLOW 0x808000
#define COLOR_GRAY 0x808080
#define COLOR_RED 0xFF0000

#define R(x) (((x) & 0xFF0000) >> 16)
#define G(x) (((x) & 0x00FF00) >> 8)
#define B(x) ((x) & 0x0000FF)

#define COLOR(r, g, b) (((r) << 16) + ((g) << 8) + (b))
#define GRAY(x) COLOR(x, x, x)

#define GRAY_SCALE(x, max) GRAY((int)(255 * (1 - (double)(x) / (max))))

typedef int color_t;

#endif  // COLOR_H_
