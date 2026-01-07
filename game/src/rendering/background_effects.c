#include "background_effects.h"

#include "animate.h"
#include "asteroid.h"
#include "geometry.h"
#include "graphics.h"
#include "sprites.h"

void init_background_asteroids(asteroid_ptr asteroids, size_t count,
                                graphics_context_ptr graphics_context) {
  for (size_t i = 0; i < count; i++) {
    asteroids[i] = create_asteroid(
        random_point(graphics_context->screen_width,
                     graphics_context->screen_height),
        random_asteroid_scale(), random_color());
  }
}

void animate_background_asteroids(asteroid_ptr asteroids, size_t count,
                                   graphics_context_ptr graphics_context,
                                   double delta_time) {
  for (size_t i = 0; i < count; i++) {
    asteroid_ptr asteroid = &asteroids[i];
    wrap_animate(graphics_context, &asteroid->position, &asteroid->velocity,
                 delta_time);
    render_asteroid(graphics_context, asteroid);
  }
}
