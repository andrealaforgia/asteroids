#include "event_system.h"

#include <string.h>

event_system_t create_event_system(void) {
  event_system_t system;
  memset(&system, 0, sizeof(event_system_t));
  return system;
}

void subscribe(event_system_ptr system, int event_type,
               event_callback_t callback, void* user_data) {
  if (event_type < 0 || event_type >= MAX_EVENT_TYPES) {
    return;
  }

  event_subscriber_list_t* list = &system->subscribers[event_type];
  if (list->callback_count >= MAX_SUBSCRIBERS_PER_EVENT) {
    return;
  }

  list->callbacks[list->callback_count] = callback;
  list->user_data[list->callback_count] = user_data;
  list->callback_count++;
}

void unsubscribe(event_system_ptr system, int event_type,
                 event_callback_t callback) {
  if (event_type < 0 || event_type >= MAX_EVENT_TYPES) {
    return;
  }

  event_subscriber_list_t* list = &system->subscribers[event_type];

  for (size_t i = 0; i < list->callback_count; i++) {
    if (list->callbacks[i] == callback) {
      // Swap with last element and decrement count
      list->callbacks[i] = list->callbacks[list->callback_count - 1];
      list->user_data[i] = list->user_data[list->callback_count - 1];
      list->callback_count--;
      return;
    }
  }
}

void publish(event_system_ptr system, const game_event_t* event) {
  if (event->type < 0 || event->type >= MAX_EVENT_TYPES) {
    return;
  }

  event_subscriber_list_t* list = &system->subscribers[event->type];

  for (size_t i = 0; i < list->callback_count; i++) {
    list->callbacks[i](event, list->user_data[i]);
  }
}

void destroy_event_system(event_system_ptr system) {
  // Nothing to free for now since we don't allocate dynamic memory
  (void)system;
}
