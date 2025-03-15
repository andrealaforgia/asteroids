#include "render.h"

#include "asteroid.h"
#include "bullet.h"
#include "coords.h"
#include "geometry.h"
#include "graphics.h"
#include "inline.h"
#include "saucer.h"
#include "sharpnel.h"
#include "ship.h"
#include "text.h"

const coords_t OBJECT_COORDS[] = {
    // asteroid 1
    {0, 4, 0},
    {4, 4, 7},
    {4, -4, 7},
    {-2, -4, 7},
    {2, -4, 7},
    {-6, -4, 8},
    {-6, 0, 8},
    {-4, 4, 7},
    {0, 8, 7},
    {4, 4, 7},
    {4, -4, 7},
    // asteroid 2
    {4, 2, 0},
    {4, 2, 7},
    {-4, 4, 7},
    {-4, -2, 7},
    {-4, 2, 7},
    {-4, -4, 7},
    {2, -4, 7},
    {-2, -4, 7},
    {4, -4, 7},
    {2, 2, 7},
    {6, -2, 8},
    {4, 6, 8},
    {-4, 4, 7},
    // asteroid 3
    {-4, 0, 0},
    {-4, -2, 7},
    {4, -6, 7},
    {4, 6, 7},
    {0, -6, 7},
    {4, 0, 7},
    {4, 6, 7},
    {0, 4, 7},
    {-4, 6, 7},
    {-6, 0, 7},
    {-6, -6, 7},
    {4, -2, 7},
    // asteroid 4
    {2, 0, 0},
    {6, 2, 7},
    {0, 2, 6},
    {-6, 4, 7},
    {-6, 0, 7},
    {2, -4, 6},
    {-6, 0, 7},
    {0, -6, 7},
    {4, -6, 7},
    {6, 2, 7},
    {2, -2, 6},
    {4, 4, 6},
    {-6, 4, 7},
    // sharpnel
    {-4, 0, 0},
    {0, 0, 15},
    {-4, -4, 0},
    {0, 0, 15},
    {4, -4, 0},
    {0, 0, 15},
    {6, 2, 0},
    {0, 0, 15},
    {4, -2, 0},
    {0, 0, 15},
    {0, 4, 0},
    {0, 0, 15},
    {2, 6, 0},
    {0, 0, 15},
    {-2, 6, 0},
    {0, 0, 15},
    {-8, -2, 0},
    {0, 0, 15},
    {-6, 2, 0},
    {0, 0, 15},
    // saucer
    {-4, 2, 0},
    {8, 0, 12},
    {6, -4, 0},
    {-20, 0, 13},
    {6, -4, 13},
    {8, 0, 12},
    {6, 4, 13},
    {-6, 4, 13},
    {-2, 4, 12},
    {-4, 0, 12},
    {-2, -4, 12},
    {-6, -4, 13},
};

const bounds_t ASTEROID_BOUNDS[] = {{0, 11}, {11, 24}, {24, 36}, {36, 49}};

const bounds_t SAUCER_BOUNDS = {69, 81};

ALWAYS_INLINE void render_object(const graphics_context_ptr graphics_context,
                                 bounds_t bounds, const point_ptr position,
                                 int scale, int color) {
  int cx = position->x;
  int cy = position->y;
  for (int i = bounds.lower; i < bounds.upper; i++) {
    int nx = cx + (OBJECT_COORDS[i].x_delta * scale);
    int ny = cy - (OBJECT_COORDS[i].y_delta * scale);
    if (OBJECT_COORDS[i].brightness > 0) {
      draw_line(graphics_context, cx, cy, nx, ny, color);
    }
    cx = nx;
    cy = ny;
  }
}

ALWAYS_INLINE void render_asteroid(const graphics_context_ptr graphics_context,
                                   const asteroid_ptr asteroid) {
  render_object(graphics_context, ASTEROID_BOUNDS[asteroid->type],
                &asteroid->position, asteroid->scale, asteroid->color);
}

ALWAYS_INLINE void render_saucer(const graphics_context_ptr graphics_context,
                                 const saucer_ptr saucer) {
  render_object(graphics_context, SAUCER_BOUNDS, &saucer->position,
                saucer->scale, COLOR_RED);
}

ALWAYS_INLINE void _render_ship(const graphics_context_ptr graphics_context,
                                const int ship_rotation_index,
                                const int ship_scale,
                                const point_ptr ship_position,
                                const bool thrusting, color_t ship_color) {
  point_t points[NUMBER_OF_POINTS];
  create_ship_points(ship_rotation_index, ship_scale, ship_position, points);
  int number_of_points = thrusting ? NUMBER_OF_POINTS : NUMBER_OF_POINTS - 2;
  for (int i = 0; i < number_of_points; i++) {
    int j = i == number_of_points - 1 ? 0 : i + 1;
    draw_line_between_points(graphics_context, &points[i], &points[j],
                             ship_color);
  }
}

ALWAYS_INLINE
void render_ship(const graphics_context_ptr graphics_context,
                 const ship_ptr ship) {
  _render_ship(graphics_context, ship->rotation_index, ship->scale,
               &ship->position, ship->thrusting, COLOR_WHITE);
}

ALWAYS_INLINE void render_destroyed_ship(
    const graphics_context_ptr graphics_context, const ship_ptr ship,
    const color_t color, int scale) {
  point_t points[NUMBER_OF_POINTS];
  create_ship_points(ship->rotation_index, scale, &ship->position, points);
  for (int i = 2; i <= 4; i++) {
    int j = (i + 1) > 4 ? 2 : i + 1;
    draw_line(graphics_context, points[i].x + rand() % 10 + 5,
              points[i].y + rand() % 10 + 5, points[j].x + rand() % 10 + 5,
              points[j].y + rand() % 10 + 5, color);
  }
}

ALWAYS_INLINE void render_lives(const graphics_context_ptr graphics_context,
                                point_t position, const int lives) {
  for (int i = 0; i < lives; i++) {
    _render_ship(graphics_context, 0, 1, &position, 0, COLOR_WHITE);
    position.x -= 20;
  }
}

ALWAYS_INLINE void render_bullet(const graphics_context_ptr graphics_context,
                                 const bullet_ptr bullet, color_t color) {
  draw_fat_pixel(graphics_context, &bullet->position, color);
}
