#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_constants.h"
#include "game_events.h"
#include "game_over_stage.h"
#include "game_settings.h"
#include "intro_stage.h"
#include "playing_stage.h"
#include "score.h"
#include "stage.h"
#include "test_allocator.h"
extern int test_ticks;
static game_t test_game(void) {
  game_t game = {0};
  game.settings = init_game_settings(false, false, 0, 0, WINDOWED, 60, 32, 5);
  game.graphics_context.screen_width = 1440;
  game.graphics_context.screen_height = 900;
  game.graphics_context.screen_center = point(720, 450);
  reset_game(&game);
  return game;
}
static void stages_release_every_allocation(void) {
  game_t game = test_game();
  for (int replay = 0; replay < 10; ++replay) {
    playing_stage_state_ptr state = create_playing_stage(&game);
    assert(state);
    destroy_playing_stage(state);
    assert(outstanding_allocations() == 0);
  }
}
static void stage_creation_cleans_up_on_failure(void) {
  game_t game = test_game();
  bool succeeded = false;
  for (int allocation = 0; allocation < 64; ++allocation) {
    fail_allocation_after(allocation);
    playing_stage_state_ptr state = create_playing_stage(&game);
    if (state) {
      destroy_playing_stage(state);
      assert(outstanding_allocations() == 0);
      succeeded = true;
      break;
    }
    assert(outstanding_allocations() == 0);
  }
  assert(succeeded);
  fail_allocation_after(-1);
}
static void count_destructions(const game_event_t* event, void* data) {
  (void)event;
  ++*(int*)data;
}
static void ship_destruction_is_a_single_transition(void) {
  game_t game = test_game();
  playing_stage_state_ptr state = create_playing_stage(&game);
  state->ship = create_ship(point(720, 450), 1);
  int count = 0;
  subscribe(&state->event_system, GAME_EVENT_SHIP_DESTROYED, count_destructions,
            &count);
  ship_controller_handle_destruction(&state->ship_controller);
  ship_controller_handle_destruction(&state->ship_controller);
  assert(count == 1);
  assert(pool_get_active_count(&state->sharpnel_system->pool) == 1);
  destroy_playing_stage(state);
}
static void saucers_wait_from_the_start_of_gameplay(void) {
  game_t game = test_game();
  test_ticks = 90000;
  playing_stage_state_ptr state = create_playing_stage(&game);
  create_saucer_if_required(&state->saucer_manager);
  assert(!is_saucer_flying(&state->saucer_manager));
  test_ticks += 30001;
  create_saucer_if_required(&state->saucer_manager);
  assert(is_saucer_flying(&state->saucer_manager));
  destroy_playing_stage(state);
}
static void spawning_finishes_on_small_screens(void) {
  game_t game = test_game();
  game.graphics_context.screen_width = 100;
  game.graphics_context.screen_height = 100;
  game.graphics_context.screen_center = point(50, 50);
  playing_stage_state_ptr state = create_playing_stage(&game);
  create_asteroids(&state->asteroid_manager, point(50, 50));
  assert(get_asteroid_count(&state->asteroid_manager) >= 1);
  destroy_playing_stage(state);
}
static void mute_restores_the_preferred_volume(void) {
  game_t game = test_game();
  game.settings = init_game_settings(false, false, 0, 0, WINDOWED, 60, 64, 5);
  assert(game_audio_volume(&game.settings) == 64);
  assert(!toggle_game_sound(&game));
  assert(game_audio_volume(&game.settings) == 0);
  assert(game.settings.volume == 64);
  assert(toggle_game_sound(&game));
  assert(game_audio_volume(&game.settings) == 64);
  game.settings = init_game_settings(false, false, 0, 0, WINDOWED, 60, 0, 5);
  assert(game_audio_volume(&game.settings) == 0);
  assert(toggle_game_sound(&game));
  assert(game_audio_volume(&game.settings) == 32);
}
static ship_t simulate_held_input(int frames) {
  game_t game = test_game();
  srand(42);
  playing_stage_state_ptr state = create_playing_stage(&game);
  ship_input_t input = {.left = true, .thrust = true};
  for (int frame = 0; frame < frames; ++frame) {
    assert(advance_playing_stage(state, input, 60.0 / frames));
  }
  ship_t ship = state->ship;
  destroy_playing_stage(state);
  assert(outstanding_allocations() == 0);
  return ship;
}
static void controls_and_motion_are_independent_of_render_fps(void) {
  ship_t baseline = simulate_held_input(60);
  int frame_rates[] = {30, 120, 144, 300};
  for (size_t i = 0; i < sizeof(frame_rates) / sizeof(frame_rates[0]); ++i) {
    ship_t ship = simulate_held_input(frame_rates[i]);
    assert(ship.rotation_index == baseline.rotation_index);
    assert(fabs(ship.position.x - baseline.position.x) < 0.000001);
    assert(fabs(ship.position.y - baseline.position.y) < 0.000001);
    assert(fabs(ship.velocity.direction.x - baseline.velocity.direction.x) <
           0.000001);
    assert(fabs(ship.velocity.direction.y - baseline.velocity.direction.y) <
           0.000001);
  }
}
static void slow_frames_still_detect_bullet_collisions(void) {
  game_t game = test_game();
  playing_stage_state_ptr state = create_playing_stage(&game);
  add_asteroid(&state->asteroid_manager, point(130, 450), SMALL_ASTEROID_SCALE);
  get_asteroid(&state->asteroid_manager, 0)->velocity.speed = 0;
  add_ship_bullet(&state->bullet_manager, point(100, 450),
                  velocity(10, vector(1, 0)));
  ship_input_t input = {0};
  assert(advance_playing_stage(state, input, 6));
  assert(get_ship_bullet_count(&state->bullet_manager) == 0);
  assert(game.score == SMALL_ASTEROID_SCORE);
  destroy_playing_stage(state);
}
static int samples;
static point_t always_the_ship_position(int width, int height) {
  ++samples;
  return point(width / 2, height / 2);
}
static void spawning_has_a_deterministic_bounded_fallback(void) {
  game_t game = test_game();
  playing_stage_state_ptr state = create_playing_stage(&game);
  state->asteroid_manager.position_source = always_the_ship_position;
  samples = 0;
  create_asteroids(&state->asteroid_manager, point(720, 450));
  assert(get_asteroid_count(&state->asteroid_manager) == 15);
  assert(samples == 15 * 64);
  assert(point_distance(&get_asteroid(&state->asteroid_manager, 0)->position,
                        &game.graphics_context.screen_center) > 192);
  destroy_playing_stage(state);
}

static void stage_factories_handle_allocation_failure(void) {
  game_t game = test_game();
  fail_allocation_after(0);
  assert(!create_intro_stage(&game));
  assert(!create_game_over_stage(&game));
  assert(!create_intro_stage_instance());
  assert(!create_game_over_stage_instance());
  assert(!create_playing_stage_instance());
  assert(outstanding_allocations() == 0);
  fail_allocation_after(-1);
}
static void firing_repeat_is_independent_of_frame_rate(void) {
  int frame_rates[] = {30, 60, 120, 144, 300};
  for (size_t i = 0; i < sizeof(frame_rates) / sizeof(frame_rates[0]); ++i) {
    game_t game = test_game();
    srand(42);
    playing_stage_state_ptr state = create_playing_stage(&game);
    state->asteroid_manager.position_source = always_the_ship_position;
    ship_input_t input = {.fire = true};
    for (int frame = 0; frame < frame_rates[i]; ++frame) {
      assert(advance_playing_stage(state, input, 60.0 / frame_rates[i]));
    }
    assert(get_ship_bullet_count(&state->bullet_manager) == 7);
    destroy_playing_stage(state);
    assert(outstanding_allocations() == 0);
  }
}

static void simulation_rejects_invalid_time_and_bounds_stalls(void) {
  game_t game = test_game();
  playing_stage_state_ptr state = create_playing_stage(&game);
  state->ship.velocity = velocity(1, vector(1, 0));
  ship_input_t input = {0};
  assert(advance_playing_stage(state, input, NAN));
  assert(advance_playing_stage(state, input, -1));
  assert(state->ship.position.x == 720);
  assert(advance_playing_stage(state, input, 600));
  assert(state->ship.position.x == 735);
  destroy_playing_stage(state);
}

static void simultaneous_collisions_consume_only_one_life(void) {
  game_t game = test_game();
  playing_stage_state_ptr state = create_playing_stage(&game);
  state->ship.creation_ticks = 0;
  int count = 0;
  subscribe(&state->event_system, GAME_EVENT_SHIP_DESTROYED, count_destructions,
            &count);
  add_asteroid(&state->asteroid_manager, state->ship.position,
               LARGE_ASTEROID_SCALE);
  get_asteroid(&state->asteroid_manager, 0)->velocity.speed = 0;
  add_saucer_bullet(&state->bullet_manager, state->ship.position,
                    state->ship.position);
  ship_input_t input = {0};
  assert(advance_playing_stage(state, input, 0.25));
  assert(count == 1);
  assert(game.lives == game.settings.initial_lives - 1);
  destroy_playing_stage(state);
}

int main(int argc, char** argv) {
  assert(argc == 2);
  if (!strcmp(argv[1], "lifecycle"))
    stages_release_every_allocation();
  else if (!strcmp(argv[1], "allocation"))
    stage_creation_cleans_up_on_failure();
  else if (!strcmp(argv[1], "destruction"))
    ship_destruction_is_a_single_transition();
  else if (!strcmp(argv[1], "saucer"))
    saucers_wait_from_the_start_of_gameplay();
  else if (!strcmp(argv[1], "spawn"))
    spawning_finishes_on_small_screens();
  else if (!strcmp(argv[1], "mute"))
    mute_restores_the_preferred_volume();
  else if (!strcmp(argv[1], "simulation"))
    controls_and_motion_are_independent_of_render_fps();
  else if (!strcmp(argv[1], "collision"))
    slow_frames_still_detect_bullet_collisions();
  else if (!strcmp(argv[1], "randomness"))
    spawning_has_a_deterministic_bounded_fallback();
  else if (!strcmp(argv[1], "factories"))
    stage_factories_handle_allocation_failure();
  else if (!strcmp(argv[1], "firing"))
    firing_repeat_is_independent_of_frame_rate();
  else if (!strcmp(argv[1], "stall"))
    simulation_rejects_invalid_time_and_bounds_stalls();
  else if (!strcmp(argv[1], "simultaneous"))
    simultaneous_collisions_consume_only_one_life();
  else
    return 1;
  return 0;
}
