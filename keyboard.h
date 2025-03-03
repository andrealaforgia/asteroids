#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <SDL.h>
#include <stdbool.h>

#include "clock.h"

typedef struct {
  const Uint8 *keys;
  int space_key_last_ticks;
  int up_key_last_ticks;
  int left_key_last_ticks;
  int right_key_last_ticks;
  int s_key_last_ticks;
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

#endif  // KEYBOARD_H_
