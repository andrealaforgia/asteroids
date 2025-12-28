#include "game_over_stage.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "animate.h"
#include "asteroid.h"
#include "audio.h"
#include "clock.h"
#include "events.h"
#include "frame.h"
#include "game.h"
#include "geometry.h"
#include "graphics.h"
#include "keyboard.h"
#include "physics.h"
#include "render.h"
#include "string.h"
#include "text.h"

#define TITLE_TEXT "GAME OVER"
#define ACTION_TEXT "PRESS RETURN TO RESTART GAME"
#define action_text_FLASHING_TICKS 750

static game_ptr game = NULL;
static graphics_context_ptr graphics_context = NULL;
static audio_context_ptr audio_context = NULL;

static int title_text_scale;
static text_dimensions_t title_text_dimensions;
static point_t title_text_position;

static int score_text_scale;
static char score_text[100] = {0};

static int action_text_scale;
static text_dimensions_t action_text_dimensions;
static point_t action_text_position;
static bool is_action_text_on = true;
static int last_action_text_ticks = 0;

static int copyright_text_scale;
static text_dimensions_t copyright_text_dimensions;

#define ASTEROIDS_COUNT 100
static asteroid_t asteroids[ASTEROIDS_COUNT];

void init_game_over_stage(const game_ptr _game) {
  game = _game;
  graphics_context = &game->graphics_context;
  audio_context = &game->audio_context;

  score_text_scale = (graphics_context->screen_height * 5) / 450;

  title_text_scale = (graphics_context->screen_height * 25) / 900;

  title_text_dimensions =
      calculate_text_dimensions(TITLE_TEXT, title_text_scale);

  title_text_position = point(
      graphics_context->screen_center.x - title_text_dimensions.width / 2,
      graphics_context->screen_center.y - graphics_context->screen_height / 6);

  action_text_scale = (graphics_context->screen_height * 10) / 900;

  action_text_dimensions =
      calculate_text_dimensions(ACTION_TEXT, action_text_scale);

  action_text_position = point(
      graphics_context->screen_center.x - action_text_dimensions.width / 2,
      graphics_context->screen_center.y + graphics_context->screen_height / 6);

  copyright_text_scale = (graphics_context->screen_height * 5) / 900;

  copyright_text_dimensions =
      calculate_text_dimensions(COPYRIGHT_TEXT, copyright_text_scale);
}

static void play_game_over_if_sound_on(void) {
  if (!game->settings.no_sound) {
    play_game_over(audio_context);
  }
}

static ALWAYS_INLINE void create_asteroids(void) {
  for (size_t i = 0; i < ASTEROIDS_COUNT; i++) {
    asteroids[i] = create_asteroid(random_point(graphics_context),
                                   random_asteroid_scale(), random_color());
  }
}

static ALWAYS_INLINE void animate_asteroids(void) {
  for (size_t i = 0; i < ASTEROIDS_COUNT; i++) {
    const asteroid_ptr asteroid = &asteroids[i];
    wrap_animate(graphics_context, &asteroid->position, &asteroid->velocity);
    render_asteroid(graphics_context, asteroid);
  }
}

static ALWAYS_INLINE void show_score(void) {
  snprintf(score_text, sizeof score_text, "SCORE %d", game->score);
  text_dimensions_t score_text_dimensions =
      calculate_text_dimensions(score_text, score_text_scale);
  point_t score_text_position =
      point(graphics_context->screen_center.x - score_text_dimensions.width / 2,
            graphics_context->screen_center.y);
  write_text(graphics_context, score_text, score_text_position,
             score_text_scale, COLOR_YELLOW);
}

static ALWAYS_INLINE void show_copyright(void) {
  write_text(graphics_context, COPYRIGHT_TEXT,
             point(graphics_context->screen_center.x -
                       copyright_text_dimensions.width / 2,
                   graphics_context->screen_height -
                       copyright_text_dimensions.height - 5),
             copyright_text_scale, COLOR_DARK_YELLOW);
}

static ALWAYS_INLINE void animate_action_text(void) {
  if (elapsed_from(last_action_text_ticks) > action_text_FLASHING_TICKS) {
    is_action_text_on = !is_action_text_on;
    last_action_text_ticks = get_clock_ticks_ms();
  }
  if (is_action_text_on) {
    write_text(graphics_context, ACTION_TEXT, action_text_position,
               action_text_scale, COLOR_WHITE);
  }
}

static ALWAYS_INLINE void show_title_text(void) {
  write_text(graphics_context, TITLE_TEXT, title_text_position,
             title_text_scale, COLOR_YELLOW);
}

game_stage_action_t handle_game_over_stage(void) {
  play_game_over_if_sound_on();

  int last_frame_ticks = get_clock_ticks_ms();

  create_asteroids();

  while (true) {
    if (elapsed_from(last_frame_ticks) < (1000 / game->settings.fps)) {
      continue;
    }
    last_frame_ticks = get_clock_ticks_ms();

    clear_frame(graphics_context);

    animate_asteroids();

    animate_action_text();

    show_title_text();

    show_score();

    show_copyright();

    render_frame(graphics_context);

    event_t event;

    while ((event = poll_event())) {
      switch (event) {
        case NO_EVENT:
        case KEY_PRESSED_EVENT:
        case OTHER_EVENT:
          break;
        case QUIT_EVENT: {
          return QUIT;
        }
      }
    }

    if (is_return_key_pressed(&game->keyboard_state)) {
      return PROGRESS;
    }

    if (is_esc_key_pressed(&game->keyboard_state)) {
      return QUIT;
    }
  }
}
