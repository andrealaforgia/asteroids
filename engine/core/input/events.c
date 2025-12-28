#include "events.h"

#include <SDL.h>

#include "inline.h"

ALWAYS_INLINE event_t poll_event(void) {
  SDL_Event e;
  if (!SDL_PollEvent(&e)) {
    return NO_EVENT;
  }
  switch (e.type) {
    case SDL_KEYDOWN:
      return KEY_PRESSED_EVENT;
    case SDL_QUIT:
      return QUIT_EVENT;
  }
  return OTHER_EVENT;
}
