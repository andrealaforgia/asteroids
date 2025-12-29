/**
 * @file game_constants.h
 * @brief Game-wide constant definitions and tuning parameters
 *
 * Defines all magic numbers and tuning parameters for the game including
 * physics constants, timing durations, entity limits, and speed values.
 * Centralizes configuration for easy gameplay balancing and tweaking.
 */

#ifndef GAME_SRC_MAIN_GAME_CONSTANTS_H_
#define GAME_SRC_MAIN_GAME_CONSTANTS_H_

// Physics constants
#define SHIP_BASE_SPEED 1.0
#define SHIP_THRUST_ACCELERATION 0.625
#define SHIP_ROTATION_STEPS 64

// Timing constants (in milliseconds)
#define SHIP_IMMUNITY_DURATION_MS 3000
#define SHIP_THRUST_DURATION_MS 90
#define SAUCER_CREATION_FREQUENCY_MS 30000
#define SAUCER_BULLET_FIRE_INTERVAL_MS 3000
#define SHIP_BULLET_MAX_AGE_MS 1000
#define SAUCER_BULLET_MAX_AGE_MS 5000
#define ACTION_TEXT_FLASH_INTERVAL_MS 750
#define SOUND_NOTIFICATION_DURATION_MS 2000

// Entity limits
#define MAX_ASTEROID_COUNT 1000
#define MAX_SHIP_BULLET_COUNT 20
#define MAX_SAUCER_BULLET_COUNT 20
#define MAX_SHARPNEL_COUNT 50

// Rendering constants
#define BULLET_PIXEL_RADIUS 2

// Geometry constants
#define SHIP_CANNON_POINT_INDEX 3

// Speed constants
#define SAUCER_BULLET_SPEED 10
#define BULLET_SPEED 10

#endif  // GAME_SRC_MAIN_GAME_CONSTANTS_H_
