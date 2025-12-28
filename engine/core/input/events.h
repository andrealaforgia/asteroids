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
