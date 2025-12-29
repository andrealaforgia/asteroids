#include "score.h"

#include "asteroid.h"
#include "event_system.h"
#include "game.h"
#include "game_events.h"

inline void score(const game_ptr game, int score) {
  if (game->score + score > MAX_SCORE) {
    game->score = MAX_SCORE;
    return;
  }
  game->score += score;
}

inline void score_large_asteroid(const game_ptr game) {
  score(game, LARGE_ASTEROID_SCORE);
}

inline void score_medium_asteroid(const game_ptr game) {
  score(game, MEDIUM_ASTEROID_SCORE);
}

inline void score_small_asteroid(const game_ptr game) {
  score(game, SMALL_ASTEROID_SCORE);
}

inline void score_large_saucer(const game_ptr game) {
  score(game, LARGE_SAUCER_SCORE);
}

inline void score_small_saucer(const game_ptr game) {
  score(game, SMALL_SAUCER_SCORE);
}

/* ---- ==== Event Subscribers ==== ---- */

static void on_asteroid_destroyed(const game_event_t* event, void* user_data) {
  game_ptr game = (game_ptr)user_data;
  asteroid_destroyed_data_t* data = (asteroid_destroyed_data_t*)event->data;

  switch (data->scale) {
    case LARGE_ASTEROID_SCALE:
      score_large_asteroid(game);
      break;
    case MEDIUM_ASTEROID_SCALE:
      score_medium_asteroid(game);
      break;
    case SMALL_ASTEROID_SCALE:
      score_small_asteroid(game);
      break;
  }
}

static void on_saucer_destroyed(const game_event_t* event, void* user_data) {
  game_ptr game = (game_ptr)user_data;
  saucer_destroyed_data_t* data = (saucer_destroyed_data_t*)event->data;

  if (data->is_big) {
    score_large_saucer(game);
  } else {
    score_small_saucer(game);
  }
}

void subscribe_score_events(event_system_ptr events, game_ptr game) {
  subscribe(events, GAME_EVENT_ASTEROID_DESTROYED, on_asteroid_destroyed,
            game);
  subscribe(events, GAME_EVENT_SAUCER_DESTROYED, on_saucer_destroyed, game);
}
