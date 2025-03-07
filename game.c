#include "game.h"

#include <stdlib.h>

#include "audio.h"
#include "game_settings.h"
#include "graphics.h"
#include "inline.h"
#include "keyboard.h"

#define INITIAL_LIVES 5

game_t init_game(game_settings_t game_settings) {
  game_t game;
  game.settings = game_settings;
  game.graphics_context =
      init_graphics_context(game.settings.display, game.settings.display_mode,
                            game.settings.window_mode);
  game.audio_context = init_audio_context();
  game.keyboard_state = init_keyboard_state();
  reset_game(&game);
  return game;
}

void terminate_game(const game_ptr game) {
  terminate_audio_context(&game->audio_context);
  terminate_graphics_context(&game->graphics_context);
}

void reset_game(const game_ptr game) {
  game->lives = INITIAL_LIVES;
  game->score = 0;
}
