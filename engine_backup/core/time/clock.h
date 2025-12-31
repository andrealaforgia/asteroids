/**
 * @file clock.h
 * @brief Time measurement and elapsed time calculations
 *
 * Provides functions for getting current time in milliseconds and
 * calculating elapsed time since a previous timestamp. Wraps SDL
 * timing functions for consistent time management.
 */

#ifndef ENGINE_CORE_TIME_CLOCK_H_
#define ENGINE_CORE_TIME_CLOCK_H_

int get_clock_ticks_ms(void);
int elapsed_from(int ticks);

#endif  // ENGINE_CORE_TIME_CLOCK_H_
