#include "ship.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "geometry.h"
#include "inline.h"

const rel_point_t COORDS[NUMBER_OF_ROTATIONS][NUMBER_OF_POINTS] = {
    // rotation 0
    {
        {4, 6},
        {-4, 6},
        {-8, 10},
        {0, -14},
        {8, 10},
        {4, 6},
        {0, 14},
        {-4, 6},
    },
    // rotation 1
    {
        {5, 6},
        {-3, 6},
        {-7, 11},
        {-1, -14},
        {9, 9},
        {5, 6},
        {1, 14},
        {-3, 6},
    },
    // rotation 2
    {
        {5, 5},
        {-3, 7},
        {-6, 11},
        {-3, -14},
        {10, 8},
        {5, 5},
        {3, 14},
        {-3, 7},
    },
    // rotation 3
    {
        {6, 5},
        {-2, 7},
        {-5, 12},
        {-4, -13},
        {11, 7},
        {6, 5},
        {4, 13},
        {-2, 7},
    },
    // rotation 4
    {
        {6, 4},
        {-1, 7},
        {-4, 12},
        {-5, -13},
        {11, 6},
        {6, 4},
        {5, 13},
        {-1, 7},
    },
    // rotation 5
    {
        {6, 3},
        {-1, 7},
        {-2, 13},
        {-7, -12},
        {12, 5},
        {6, 3},
        {7, 12},
        {-1, 7},
    },
    // rotation 6
    {
        {7, 3},
        {0, 7},
        {-1, 13},
        {-8, -12},
        {12, 4},
        {7, 3},
        {8, 12},
        {0, 7},
    },
    // rotation 7
    {
        {7, 2},
        {1, 7},
        {0, 13},
        {-9, -11},
        {13, 3},
        {7, 2},
        {9, 11},
        {1, 7},
    },
    // rotation 8
    {
        {7, 1},
        {1, 7},
        {1, 13},
        {-10, -10},
        {13, 1},
        {7, 1},
        {10, 10},
        {1, 7},
    },
    // rotation 9
    {
        {7, 1},
        {2, 7},
        {3, 13},
        {-11, -9},
        {13, 0},
        {7, 1},
        {11, 9},
        {2, 7},
    },
    // rotation 10
    {
        {7, 0},
        {3, 7},
        {4, 12},
        {-12, -8},
        {13, -1},
        {7, 0},
        {12, 8},
        {3, 7},
    },
    // rotation 11
    {
        {7, -1},
        {3, 6},
        {5, 12},
        {-12, -7},
        {13, -2},
        {7, -1},
        {12, 7},
        {3, 6},
    },
    // rotation 12
    {
        {7, -1},
        {4, 6},
        {6, 11},
        {-13, -5},
        {12, -4},
        {7, -1},
        {13, 5},
        {4, 6},
    },
    // rotation 13
    {
        {7, -2},
        {5, 6},
        {7, 11},
        {-13, -4},
        {12, -5},
        {7, -2},
        {13, 4},
        {5, 6},
    },
    // rotation 14
    {
        {7, -3},
        {5, 5},
        {8, 10},
        {-14, -3},
        {11, -6},
        {7, -3},
        {14, 3},
        {5, 5},
    },
    // rotation 15
    {
        {6, -3},
        {6, 5},
        {9, 9},
        {-14, -1},
        {11, -7},
        {6, -3},
        {14, 1},
        {6, 5},
    },
    // rotation 16
    {
        {6, -4},
        {6, 4},
        {10, 8},
        {-14, 0},
        {10, -8},
        {6, -4},
        {14, 0},
        {6, 4},
    },
    // rotation 17
    {
        {6, -5},
        {6, 3},
        {11, 7},
        {-14, 1},
        {9, -9},
        {6, -5},
        {14, -1},
        {6, 3},
    },
    // rotation 18
    {
        {5, -5},
        {7, 3},
        {11, 6},
        {-14, 3},
        {8, -10},
        {5, -5},
        {14, -3},
        {7, 3},
    },
    // rotation 19
    {
        {5, -6},
        {7, 2},
        {12, 5},
        {-13, 4},
        {7, -11},
        {5, -6},
        {13, -4},
        {7, 2},
    },
    // rotation 20
    {
        {4, -6},
        {7, 1},
        {12, 4},
        {-13, 5},
        {6, -11},
        {4, -6},
        {13, -5},
        {7, 1},
    },
    // rotation 21
    {
        {3, -6},
        {7, 1},
        {13, 2},
        {-12, 7},
        {5, -12},
        {3, -6},
        {12, -7},
        {7, 1},
    },
    // rotation 22
    {
        {3, -7},
        {7, 0},
        {13, 1},
        {-12, 8},
        {4, -12},
        {3, -7},
        {12, -8},
        {7, 0},
    },
    // rotation 23
    {
        {2, -7},
        {7, -1},
        {13, 0},
        {-11, 9},
        {3, -13},
        {2, -7},
        {11, -9},
        {7, -1},
    },
    // rotation 24
    {
        {1, -7},
        {7, -1},
        {13, -1},
        {-10, 10},
        {1, -13},
        {1, -7},
        {10, -10},
        {7, -1},
    },
    // rotation 25
    {
        {1, -7},
        {7, -2},
        {13, -3},
        {-9, 11},
        {0, -13},
        {1, -7},
        {9, -11},
        {7, -2},
    },
    // rotation 26
    {
        {0, -7},
        {7, -3},
        {12, -4},
        {-8, 12},
        {-1, -13},
        {0, -7},
        {8, -12},
        {7, -3},
    },
    // rotation 27
    {
        {-1, -7},
        {6, -3},
        {12, -5},
        {-7, 12},
        {-2, -13},
        {-1, -7},
        {7, -12},
        {6, -3},
    },
    // rotation 28
    {
        {-1, -7},
        {6, -4},
        {11, -6},
        {-5, 13},
        {-4, -12},
        {-1, -7},
        {5, -13},
        {6, -4},
    },
    // rotation 29
    {
        {-2, -7},
        {6, -5},
        {11, -7},
        {-4, 13},
        {-5, -12},
        {-2, -7},
        {4, -13},
        {6, -5},
    },
    // rotation 30
    {
        {-3, -7},
        {5, -5},
        {10, -8},
        {-3, 14},
        {-6, -11},
        {-3, -7},
        {3, -14},
        {5, -5},
    },
    // rotation 31
    {
        {-3, -6},
        {5, -6},
        {9, -9},
        {-1, 14},
        {-7, -11},
        {-3, -6},
        {1, -14},
        {5, -6},
    },
    // rotation 32
    {
        {-4, -6},
        {4, -6},
        {8, -10},
        {0, 14},
        {-8, -10},
        {-4, -6},
        {0, -14},
        {4, -6},
    },
    // rotation 33
    {
        {-5, -6},
        {3, -6},
        {7, -11},
        {1, 14},
        {-9, -9},
        {-5, -6},
        {-1, -14},
        {3, -6},
    },
    // rotation 34
    {
        {-5, -5},
        {3, -7},
        {6, -11},
        {3, 14},
        {-10, -8},
        {-5, -5},
        {-3, -14},
        {3, -7},
    },
    // rotation 35
    {
        {-6, -5},
        {2, -7},
        {5, -12},
        {4, 13},
        {-11, -7},
        {-6, -5},
        {-4, -13},
        {2, -7},
    },
    // rotation 36
    {
        {-6, -4},
        {1, -7},
        {4, -12},
        {5, 13},
        {-11, -6},
        {-6, -4},
        {-5, -13},
        {1, -7},
    },
    // rotation 37
    {
        {-6, -3},
        {1, -7},
        {2, -13},
        {7, 12},
        {-12, -5},
        {-6, -3},
        {-7, -12},
        {1, -7},
    },
    // rotation 38
    {
        {-7, -3},
        {0, -7},
        {1, -13},
        {8, 12},
        {-12, -4},
        {-7, -3},
        {-8, -12},
        {0, -7},
    },
    // rotation 39
    {
        {-7, -2},
        {-1, -7},
        {0, -13},
        {9, 11},
        {-13, -3},
        {-7, -2},
        {-9, -11},
        {-1, -7},
    },
    // rotation 40
    {
        {-7, -1},
        {-1, -7},
        {-1, -13},
        {10, 10},
        {-13, -1},
        {-7, -1},
        {-10, -10},
        {-1, -7},
    },
    // rotation 41
    {
        {-7, -1},
        {-2, -7},
        {-3, -13},
        {11, 9},
        {-13, 0},
        {-7, -1},
        {-11, -9},
        {-2, -7},
    },
    // rotation 42
    {
        {-7, 0},
        {-3, -7},
        {-4, -12},
        {12, 8},
        {-13, 1},
        {-7, 0},
        {-12, -8},
        {-3, -7},
    },
    // rotation 43
    {
        {-7, 1},
        {-3, -6},
        {-5, -12},
        {12, 7},
        {-13, 2},
        {-7, 1},
        {-12, -7},
        {-3, -6},
    },
    // rotation 44
    {
        {-7, 1},
        {-4, -6},
        {-6, -11},
        {13, 5},
        {-12, 4},
        {-7, 1},
        {-13, -5},
        {-4, -6},
    },
    // rotation 45
    {
        {-7, 2},
        {-5, -6},
        {-7, -11},
        {13, 4},
        {-12, 5},
        {-7, 2},
        {-13, -4},
        {-5, -6},
    },
    // rotation 46
    {
        {-7, 3},
        {-5, -5},
        {-8, -10},
        {14, 3},
        {-11, 6},
        {-7, 3},
        {-14, -3},
        {-5, -5},
    },
    // rotation 47
    {
        {-6, 3},
        {-6, -5},
        {-9, -9},
        {14, 1},
        {-11, 7},
        {-6, 3},
        {-14, -1},
        {-6, -5},
    },
    // rotation 48
    {
        {-6, 4},
        {-6, -4},
        {-10, -8},
        {14, 0},
        {-10, 8},
        {-6, 4},
        {-14, 0},
        {-6, -4},
    },
    // rotation 49
    {
        {-6, 5},
        {-6, -3},
        {-11, -7},
        {14, -1},
        {-9, 9},
        {-6, 5},
        {-14, 1},
        {-6, -3},
    },
    // rotation 50
    {
        {-5, 5},
        {-7, -3},
        {-11, -6},
        {14, -3},
        {-8, 10},
        {-5, 5},
        {-14, 3},
        {-7, -3},
    },
    // rotation 51
    {
        {-5, 6},
        {-7, -2},
        {-12, -5},
        {13, -4},
        {-7, 11},
        {-5, 6},
        {-13, 4},
        {-7, -2},
    },
    // rotation 52
    {
        {-4, 6},
        {-7, -1},
        {-12, -4},
        {13, -5},
        {-6, 11},
        {-4, 6},
        {-13, 5},
        {-7, -1},
    },
    // rotation 53
    {
        {-3, 6},
        {-7, -1},
        {-13, -2},
        {12, -7},
        {-5, 12},
        {-3, 6},
        {-12, 7},
        {-7, -1},
    },
    // rotation 54
    {
        {-3, 7},
        {-7, 0},
        {-13, -1},
        {12, -8},
        {-4, 12},
        {-3, 7},
        {-12, 8},
        {-7, 0},
    },
    // rotation 55
    {
        {-2, 7},
        {-7, 1},
        {-13, 0},
        {11, -9},
        {-3, 13},
        {-2, 7},
        {-11, 9},
        {-7, 1},
    },
    // rotation 56
    {
        {-1, 7},
        {-7, 1},
        {-13, 1},
        {10, -10},
        {-1, 13},
        {-1, 7},
        {-10, 10},
        {-7, 1},
    },
    // rotation 57
    {
        {-1, 7},
        {-7, 2},
        {-13, 3},
        {9, -11},
        {0, 13},
        {-1, 7},
        {-9, 11},
        {-7, 2},
    },
    // rotation 58
    {
        {0, 7},
        {-7, 3},
        {-12, 4},
        {8, -12},
        {1, 13},
        {0, 7},
        {-8, 12},
        {-7, 3},
    },
    // rotation 59
    {
        {1, 7},
        {-6, 3},
        {-12, 5},
        {7, -12},
        {2, 13},
        {1, 7},
        {-7, 12},
        {-6, 3},
    },
    // rotation 60
    {
        {1, 7},
        {-6, 4},
        {-11, 6},
        {5, -13},
        {4, 12},
        {1, 7},
        {-5, 13},
        {-6, 4},
    },
    // rotation 61
    {
        {2, 7},
        {-6, 5},
        {-11, 7},
        {4, -13},
        {5, 12},
        {2, 7},
        {-4, 13},
        {-6, 5},
    },
    // rotation 62
    {
        {3, 7},
        {-5, 5},
        {-10, 8},
        {3, -14},
        {6, 11},
        {3, 7},
        {-3, 14},
        {-5, 5},
    },
    // rotation 63
    {
        {3, 6},
        {-5, 6},
        {-9, 9},
        {1, -14},
        {7, 11},
        {3, 6},
        {-1, 14},
        {-5, 6},
    }};

ALWAYS_INLINE double calculate_ship_angle(const ship_ptr ship) {
  return ship->rotation_index * M_PI / (NUMBER_OF_ROTATIONS / 2) + M_PI / 2;
}

ALWAYS_INLINE vector_t create_rotation_vector(const ship_ptr ship) {
  double angle = calculate_ship_angle(ship);
  return vector(cos(angle), -sin(angle));
}

ALWAYS_INLINE ship_t create_ship(point_t position, int scale) {
  ship_t ship;
  ship.state = ACTIVE;
  ship.position = position;
  ship.velocity = velocity(0, vector(0, 0));
  ship.scale = scale;
  ship.rotation_index = 0;
  ship.rotation_vector = create_rotation_vector(&ship);
  ship.last_thrust_ticks = get_clock_ticks_ms();
  ship.thrusting = false;
  ship.creation_ticks = get_clock_ticks_ms();
  return ship;
}

ALWAYS_INLINE void destroy_ship(const ship_ptr ship) {
  ship->state = DESTROYED;
}

ALWAYS_INLINE void create_ship_points(const int ship_rotation_index,
                                      const int ship_scale,
                                      const point_ptr ship_position,
                                      const point_ptr points) {
  for (int i = 0; i < NUMBER_OF_POINTS; i++) {
    rel_point_t rp = COORDS[ship_rotation_index][i];
    rp.x_delta *= ship_scale;
    rp.y_delta *= ship_scale;
    points[i] = relative_point(ship_position, &rp);
  }
}

ALWAYS_INLINE void rotate_ship_left(const ship_ptr ship) {
  ship->rotation_index = (ship->rotation_index == MAX_ROTATION_INDEX)
                             ? 0
                             : ship->rotation_index + 1;
  ship->rotation_vector = create_rotation_vector(ship);
}

ALWAYS_INLINE void rotate_ship_right(const ship_ptr ship) {
  ship->rotation_index = (ship->rotation_index == 0) ? MAX_ROTATION_INDEX
                                                     : ship->rotation_index - 1;
  ship->rotation_vector = create_rotation_vector(ship);
}

ALWAYS_INLINE point_t get_cannon_position(const ship_ptr ship) {
  rel_point_t rp = COORDS[ship->rotation_index][3];
  rp.x_delta *= ship->scale;
  rp.y_delta *= ship->scale;
  return relative_point(&ship->position, &rp);
}

ALWAYS_INLINE void accelerate_ship(const ship_ptr ship) {
  double angle = calculate_ship_angle(ship);
  ship->velocity.speed = 1;
  ship->velocity.direction.x += 0.625f * cos(angle);
  ship->velocity.direction.y -= 0.625f * sin(angle);
  ship->thrusting = true;
  ship->last_thrust_ticks = get_clock_ticks_ms();
}
