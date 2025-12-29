#ifndef GAME_SRC_STAGES_STAGE_H_
#define GAME_SRC_STAGES_STAGE_H_

#include "game.h"

// Forward declaration
typedef struct stage_t stage_t;

struct stage_t {
  void* state;  // Stage-specific state

  void (*init)(stage_t* stage, game_ptr game);
  game_stage_action_t (*update)(stage_t* stage);
  void (*cleanup)(stage_t* stage);

  const char* name;  // For debugging
};

typedef stage_t* stage_ptr;

// Factory functions for creating stages
stage_ptr create_intro_stage_instance(void);
stage_ptr create_playing_stage_instance(void);
stage_ptr create_game_over_stage_instance(void);

// Common stage operations
void destroy_stage(stage_ptr stage);

#endif  // GAME_SRC_STAGES_STAGE_H_
