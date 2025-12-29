/**
 * @file score.h
 * @brief Scoring system and point values
 *
 * Defines point values for destroying different entities and provides
 * functions for updating player score. Integrates with the event system
 * to automatically award points when entities are destroyed.
 */

#ifndef GAME_SRC_SCORING_SCORE_H_
#define GAME_SRC_SCORING_SCORE_H_

#include "event_system.h"
#include "game.h"

#define MAX_SCORE 999999999
#define LARGE_ASTEROID_SCORE 20
#define MEDIUM_ASTEROID_SCORE 50
#define SMALL_ASTEROID_SCORE 100
#define LARGE_SAUCER_SCORE 200
#define SMALL_SAUCER_SCORE 1000

void score(const game_ptr game, int score);
void score_large_asteroid(const game_ptr game);
void score_medium_asteroid(const game_ptr game);
void score_small_asteroid(const game_ptr game);
void score_large_saucer(const game_ptr game);
void score_small_saucer(const game_ptr game);

// Event subscriber functions
void subscribe_score_events(event_system_ptr events, game_ptr game);

#endif  // GAME_SRC_SCORING_SCORE_H_
