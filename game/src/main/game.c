#include "game.h"

#include <stdlib.h>

#include "audio.h"
#include "game_audio.h"
#include "game_settings.h"
#include "graphics.h"
#include "inline.h"
#include "keyboard.h"

game_t init_game(game_settings_t game_settings) {
  game_t game;
  game.settings = game_settings;
  game.graphics_context =
      init_graphics_context(game.settings.display, game.settings.display_mode,
                            game.settings.window_mode, game.settings.vsync);
  game.audio_context =
      init_audio_context(SOUND_COUNT, game_audio_volume(&game.settings));
  init_game_audio(&game.audio_context);
  game.keyboard_state = init_keyboard_state();
  reset_game(&game);
  return game;
}

void terminate_game(const game_ptr game) {
  terminate_audio_context(&game->audio_context);
  terminate_graphics_context(&game->graphics_context);
}

void reset_game(const game_ptr game) {
  game->lives = game->settings.initial_lives;
  game->score = 0;
}

bool toggle_game_sound(game_ptr game) {
  game->settings.muted = !game->settings.muted;
  set_audio_volume(game_audio_volume(&game->settings));
  return !game->settings.muted;
}
