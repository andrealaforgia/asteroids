/**
 * @file game_hud.h
 * @brief Heads-up display for in-game information
 *
 * Manages and renders the game HUD including player lives, score,
 * and optional FPS counter. Coordinates layout and updates for all
 * on-screen status information during gameplay.
 */

#ifndef GAME_SRC_RENDERING_GAME_HUD_H_
#define GAME_SRC_RENDERING_GAME_HUD_H_

#include "fps_tracker.h"
#include "game.h"
#include "graphics.h"

typedef struct {
  graphics_context_ptr graphics_context;
  game_ptr game;
  fps_tracker_t fps_tracker;
} game_hud_t;

typedef game_hud_t* game_hud_ptr;

// Initialize the game HUD
void init_game_hud(game_hud_ptr hud, game_ptr game,
                   graphics_context_ptr graphics_context);

// Reset HUD state (e.g., FPS tracker)
void reset_game_hud(game_hud_ptr hud);

// Render the complete HUD (lives, score, fps if enabled)
void render_hud(game_hud_ptr hud, int ship_scale);

#endif  // GAME_SRC_RENDERING_GAME_HUD_H_
