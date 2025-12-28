#ifndef SHIP_H_
#define SHIP_H_

#include <stdbool.h>

#include "geometry.h"
#include "physics.h"

#define NUMBER_OF_POINTS 8
#define NUMBER_OF_ROTATIONS 64
#define MAX_ROTATION_INDEX (NUMBER_OF_ROTATIONS - 1)

typedef enum { ACTIVE, DESTROYED } ship_state_t;

typedef struct {
  ship_state_t state;
  point_t position;
  int rotation_index;
  velocity_t velocity;
  int scale;
  bool thrusting;
  int last_thrust_ticks;
  vector_t rotation_vector;
  int creation_ticks;
} ship_t, *ship_ptr;

ship_t create_ship(point_t position, int scale);
double get_ship_angle(const ship_ptr ship);
point_t get_cannon_position(const ship_ptr ship);
void create_ship_points(const int ship_rotation_index, const int ship_scale,
                        const point_ptr ship_position, const point_ptr points);
void rotate_ship_left(const ship_ptr ship);
void rotate_ship_right(const ship_ptr ship);
void accelerate_ship(const ship_ptr ship);
void destroy_ship(const ship_ptr ship);

#endif  // SHIP_H_
