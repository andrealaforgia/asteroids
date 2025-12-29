/**
 * @file event_system.h
 * @brief Publisher-subscriber event system for game events
 *
 * Implements a simple event bus allowing components to publish and subscribe
 * to game events. Supports multiple subscribers per event type with user data
 * for decoupled communication between game systems.
 */

#ifndef ENGINE_CORE_EVENTS_EVENT_SYSTEM_H_
#define ENGINE_CORE_EVENTS_EVENT_SYSTEM_H_

#include <stddef.h>

#define MAX_SUBSCRIBERS_PER_EVENT 32
#define MAX_EVENT_TYPES 256

// Event structure
typedef struct {
  int type;
  void* data;
  size_t data_size;
} game_event_t;

// Event callback signature
typedef void (*event_callback_t)(const game_event_t* event, void* user_data);

// Subscriber list for a specific event type
typedef struct {
  event_callback_t callbacks[MAX_SUBSCRIBERS_PER_EVENT];
  void* user_data[MAX_SUBSCRIBERS_PER_EVENT];
  size_t callback_count;
} event_subscriber_list_t;

// Event system
typedef struct {
  event_subscriber_list_t subscribers[MAX_EVENT_TYPES];
} event_system_t;

typedef event_system_t* event_system_ptr;

// Event system operations
event_system_t create_event_system(void);
void subscribe(event_system_ptr system, int event_type,
               event_callback_t callback, void* user_data);
void unsubscribe(event_system_ptr system, int event_type,
                 event_callback_t callback);
void publish(event_system_ptr system, const game_event_t* event);
void destroy_event_system(event_system_ptr system);

#endif  // ENGINE_CORE_EVENTS_EVENT_SYSTEM_H_
