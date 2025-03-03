#include "intro_stage.h"

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
#include "saucer.h"
#include "text.h"

#define TITLE_TEXT "ASTEROIDS"
#define ACTION_TEXT "PRESS SPACE TO START"
#define ACTION_TEXT_FLASHING_TICKS 750

#define INSTRUCTIONS_TEXT_0 "        PRESS UP OR K TO THRUST SHIP        "
#define INSTRUCTIONS_TEXT_1 "PRESS LEFT OR H OR RIGHT OR L TO ROTATE SHIP"
#define INSTRUCTIONS_TEXT_2 "            PRESS SPACE TO SHOOT            "
#define INSTRUCTIONS_TEXT_3 "     PRESS S TO ENABLE OR DISABLE SOUND     "
#define INSTRUCTIONS_TEXT_4 "              PRESS ESC TO EXIT             "

#define ASTEROIDS_COUNT 150

static game_ptr game = NULL;
static graphics_context_ptr graphics_context = NULL;

static int title_text_scale;
static text_dimensions_t title_text_dimensions;
static point_t title_text_position;

static int instructions_text_scale;
static text_dimensions_t instructions_text_dimensions;
static point_t instructions_text_0_position;
static point_t instructions_text_1_position;
static point_t instructions_text_2_position;
static point_t instructions_text_3_position;
static point_t instructions_text_4_position;

static int action_text_scale;
static text_dimensions_t action_text_dimensions;
static point_t action_text_position;

static text_dimensions_t copyright_text_dimensions;
static int copyright_text_scale;

void init_intro_stage(const game_ptr _game) {
  game = _game;
  graphics_context = &game->graphics_context;

  title_text_scale = (graphics_context->screen_height * 25) / 900;
  title_text_dimensions =
      calculate_text_dimensions(TITLE_TEXT, title_text_scale);
  title_text_position = point(
      graphics_context->screen_center.x - title_text_dimensions.width / 2,
      graphics_context->screen_center.y - graphics_context->screen_height / 3);

  action_text_scale = (graphics_context->screen_height * 10) / 900;
  action_text_dimensions =
      calculate_text_dimensions(ACTION_TEXT, action_text_scale);
  action_text_position = point(
      graphics_context->screen_center.x - action_text_dimensions.width / 2,
      graphics_context->screen_center.y + graphics_context->screen_height / 6);

  instructions_text_scale = (graphics_context->screen_height * 10) / 900;
  instructions_text_dimensions =
      calculate_text_dimensions(INSTRUCTIONS_TEXT_0, instructions_text_scale);
  instructions_text_0_position = point(
      graphics_context->screen_center.x -
          instructions_text_dimensions.width / 2,
      graphics_context->screen_center.y - graphics_context->screen_height / 4);

  instructions_text_1_position = point(
      graphics_context->screen_center.x -
          instructions_text_dimensions.width / 2,
      graphics_context->screen_center.y - graphics_context->screen_height / 4 +
          instructions_text_dimensions.height +
          instructions_text_dimensions.height / 2);

  instructions_text_2_position = point(
      graphics_context->screen_center.x -
          instructions_text_dimensions.width / 2,
      graphics_context->screen_center.y - graphics_context->screen_height / 4 +
          instructions_text_dimensions.height * 2 +
          2 * instructions_text_dimensions.height / 2);

  instructions_text_3_position = point(
      graphics_context->screen_center.x -
          instructions_text_dimensions.width / 2,
      graphics_context->screen_center.y - graphics_context->screen_height / 4 +
          instructions_text_dimensions.height * 3 +
          3 * instructions_text_dimensions.height / 2);

  instructions_text_4_position = point(
      graphics_context->screen_center.x -
          instructions_text_dimensions.width / 2,
      graphics_context->screen_center.y - graphics_context->screen_height / 4 +
          instructions_text_dimensions.height * 4 +
          4 * instructions_text_dimensions.height / 2);

  copyright_text_scale = (graphics_context->screen_height * 5) / 900;
  copyright_text_dimensions =
      calculate_text_dimensions(COPYRIGHT_TEXT, copyright_text_scale);
}

game_stage_action_t handle_intro_stage(void) {
  int last_action_text_ticks = get_clock_ticks_ms();

  bool is_action_text_on = true;

  asteroid_t asteroids[ASTEROIDS_COUNT];
  for (int i = 0; i < ASTEROIDS_COUNT; i++) {
    asteroids[i] = create_asteroid(random_point(graphics_context),
                                   random_asteroid_scale(), random_color());
  }

  int last_frame_ticks = get_clock_ticks_ms();

  while (true) {
    if (elapsed_from(last_frame_ticks) < (1000 / game->settings.fps)) {
      continue;
    }
    last_frame_ticks = get_clock_ticks_ms();

    clear_frame(graphics_context);

    for (int i = 0; i < ASTEROIDS_COUNT; i++) {
      wrap_animate(graphics_context, &asteroids[i].position,
                   &asteroids[i].velocity);
      render_asteroid(graphics_context, &asteroids[i]);
    }

    write_text(graphics_context, INSTRUCTIONS_TEXT_0,
               instructions_text_0_position, instructions_text_scale,
               COLOR_GRAY);

    write_text(graphics_context, INSTRUCTIONS_TEXT_1,
               instructions_text_1_position, instructions_text_scale,
               COLOR_GRAY);

    write_text(graphics_context, INSTRUCTIONS_TEXT_2,
               instructions_text_2_position, instructions_text_scale,
               COLOR_GRAY);

    write_text(graphics_context, INSTRUCTIONS_TEXT_3,
               instructions_text_3_position, instructions_text_scale,
               COLOR_GRAY);

    write_text(graphics_context, INSTRUCTIONS_TEXT_4,
               instructions_text_4_position, instructions_text_scale,
               COLOR_GRAY);

    if (elapsed_from(last_action_text_ticks) > ACTION_TEXT_FLASHING_TICKS) {
      is_action_text_on = !is_action_text_on;
      last_action_text_ticks = get_clock_ticks_ms();
    }

    write_text(graphics_context, TITLE_TEXT, title_text_position,
               title_text_scale, COLOR_YELLOW);

    if (is_action_text_on) {
      write_text(graphics_context, ACTION_TEXT, action_text_position,
                 action_text_scale, COLOR_WHITE);
    }

    write_text(graphics_context, COPYRIGHT_TEXT,
               point(graphics_context->screen_center.x -
                         copyright_text_dimensions.width / 2,
                     graphics_context->screen_height -
                         copyright_text_dimensions.height - 5),
               copyright_text_scale, COLOR_DARK_YELLOW);

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

    if (is_space_key_pressed(&game->keyboard_state)) {
      return PROGRESS;
    }

    if (is_esc_key_pressed(&game->keyboard_state)) {
      return QUIT;
    }
  }
}
