/**
 * @file keyboard.h
 * @brief Keyboard input handling with debouncing
 *
 * Provides functions for detecting keyboard input with built-in
 * debouncing to prevent accidental double-presses. Maintains timing
 * state for each monitored key to ensure clean input detection.
 */

#ifndef ENGINE_CORE_INPUT_KEYBOARD_H_
#define ENGINE_CORE_INPUT_KEYBOARD_H_

#include <SDL.h>
#include <stdbool.h>

#include "clock.h"

typedef struct {
  const Uint8* keys;
  int space_key_last_ticks;
  int up_key_last_ticks;
  int left_key_last_ticks;
  int right_key_last_ticks;
  int s_key_last_ticks;
  int f11_key_last_ticks;
  int return_key_last_ticks;
} keyboard_state_t, *keyboard_state_ptr;

keyboard_state_t init_keyboard_state(void);
void print_keyboard_state(keyboard_state_ptr keyboard_state);
bool is_space_key_pressed(const keyboard_state_ptr keyboard_state);
bool is_up_key_pressed(const keyboard_state_ptr keyboard_state);
bool is_left_key_pressed(const keyboard_state_ptr keyboard_state);
bool is_right_key_pressed(const keyboard_state_ptr keyboard_state);
bool is_esc_key_pressed(const keyboard_state_ptr keyboard_state);
bool is_s_key_pressed(const keyboard_state_ptr keyboard_state);
bool is_return_key_pressed(const keyboard_state_ptr keyboard_state);
bool is_f11_key_pressed(const keyboard_state_ptr keyboard_state);

#endif  // ENGINE_CORE_INPUT_KEYBOARD_H_
