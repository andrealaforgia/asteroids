#ifndef GAME_SRC_STAGES_PLAYING_STAGE_H_
#define GAME_SRC_STAGES_PLAYING_STAGE_H_

#include "asteroid_manager.h"
#include "bullet_manager.h"
#include "event_system.h"
#include "game.h"
#include "game_hud.h"
#include "saucer_manager.h"
#include "ship_controller.h"
#include "sharpnel.h"
#include "ship.h"

typedef struct {
  game_ptr game;
  graphics_context_ptr graphics_context;
  audio_context_ptr audio_context;
  event_system_t event_system;
  asteroid_manager_t asteroid_manager;
  bullet_manager_t bullet_manager;
  saucer_manager_t saucer_manager;
  game_hud_t game_hud;
  sharpnel_system_ptr sharpnel_system;
  ship_t ship;
  ship_controller_t ship_controller;
} playing_stage_state_t;

typedef playing_stage_state_t* playing_stage_state_ptr;

playing_stage_state_ptr create_playing_stage(game_ptr game);
void destroy_playing_stage(playing_stage_state_ptr state);
game_stage_action_t handle_playing_stage(playing_stage_state_ptr state);

#endif  // GAME_SRC_STAGES_PLAYING_STAGE_H_
