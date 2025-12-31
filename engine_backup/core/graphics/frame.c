#include "frame.h"

#include <SDL.h>

#include "inline.h"

ALWAYS_INLINE void clear_frame(const graphics_context_ptr graphics_context) {
  SDL_SetRenderDrawColor(graphics_context->renderer, 0, 0, 0, 255);
  SDL_RenderClear(graphics_context->renderer);
}

ALWAYS_INLINE void render_frame(const graphics_context_ptr graphics_context) {
  SDL_RenderPresent(graphics_context->renderer);
}
