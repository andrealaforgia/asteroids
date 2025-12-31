/**
 * @file events.h
 * @brief SDL event polling and classification
 *
 * Provides a simplified event system that wraps SDL event polling
 * and classifies events into common categories. Used to detect
 * user input and system events like quit requests.
 */

#ifndef ENGINE_CORE_INPUT_EVENTS_H_
#define ENGINE_CORE_INPUT_EVENTS_H_

typedef enum {
  NO_EVENT = 0,
  QUIT_EVENT,
  KEY_PRESSED_EVENT,
  OTHER_EVENT,
} event_t;

event_t poll_event(void);

#endif  // ENGINE_CORE_INPUT_EVENTS_H_
