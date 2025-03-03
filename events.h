#ifndef EVENTS_H_
#define EVENTS_H_

typedef enum {
  NO_EVENT = 0,
  QUIT_EVENT,
  KEY_PRESSED_EVENT,
  OTHER_EVENT,
} event_t;

event_t poll_event(void);

#endif  // EVENTS_H_
