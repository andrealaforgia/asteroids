/**
 * @file game_events.h
 * @brief Game event type definitions and data structures
 *
 * Defines all game-specific event types like asteroid destroyed, ship
 * destroyed, and saucer destroyed. Includes event data structures that
 * carry information about each event for subscribers to process.
 */

#ifndef GAME_SRC_EVENTS_GAME_EVENTS_H_
#define GAME_SRC_EVENTS_GAME_EVENTS_H_

#include "geometry.h"

// Game event types
typedef enum {
  GAME_EVENT_ASTEROID_DESTROYED = 0,
  GAME_EVENT_SHIP_DESTROYED,
  GAME_EVENT_SAUCER_DESTROYED
} game_event_type_t;

// Event data structures
typedef struct {
  point_t position;
  int scale;
} asteroid_destroyed_data_t;

typedef struct {
  point_t position;
} ship_destroyed_data_t;

typedef struct {
  point_t position;
  bool is_big;
} saucer_destroyed_data_t;

#endif  // GAME_SRC_EVENTS_GAME_EVENTS_H_
