#ifndef GAME_H_
#define GAME_H_

#include "audio.h"
#include "game_settings.h"
#include "graphics.h"
#include "keyboard.h"

#define COPYRIGHT_TEXT                                                     \
  "2023 ASTEROIDS REMAKE BY ANDREA LAFORGIA THE ORIGINAL GAME IS c ATARI " \
  "1979"

typedef enum { INTRO, PLAYING, GAME_OVER } game_stage_t;

typedef enum { PROGRESS, QUIT } game_stage_action_t;

typedef struct {
  int score;
  int lives;
  game_settings_t settings;
  graphics_context_t graphics_context;
  keyboard_state_t keyboard_state;
  audio_context_t audio_context;
} game_t, *game_ptr;

game_t init_game(game_settings_t game_settings);
void terminate_game(const game_ptr game);
void reset_game(const game_ptr game);

#endif  // GAME_H_
