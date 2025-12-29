#ifndef GAME_SRC_RENDERING_BACKGROUND_EFFECTS_H_
#define GAME_SRC_RENDERING_BACKGROUND_EFFECTS_H_

#include <stddef.h>

#include "asteroid.h"
#include "graphics.h"

/**
 * Background Effects - Reusable background animation for non-gameplay screens
 *
 * Provides animated asteroid backgrounds for intro and game over screens.
 * The asteroids are purely decorative and do not interact with gameplay.
 *
 * Usage:
 *   asteroid_t asteroids[150];
 *   init_background_asteroids(asteroids, 150, graphics_context);
 *
 *   // In game loop:
 *   animate_background_asteroids(asteroids, 150, graphics_context, delta_time);
 */

// Initialize background asteroids with random positions, scales, and colors
void init_background_asteroids(asteroid_ptr asteroids, size_t count,
                                graphics_context_ptr graphics_context);

// Update and render background asteroids
void animate_background_asteroids(asteroid_ptr asteroids, size_t count,
                                   graphics_context_ptr graphics_context,
                                   double delta_time);

#endif  // GAME_SRC_RENDERING_BACKGROUND_EFFECTS_H_
