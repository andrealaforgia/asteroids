/**
 * @file window_mode.h
 * @brief Window display mode enumeration
 *
 * Defines the available window modes for the game including windowed,
 * fullscreen, borderless, and maximized. Used during graphics context
 * initialization to configure the display.
 */

#ifndef ENGINE_CORE_GRAPHICS_WINDOW_MODE_H_
#define ENGINE_CORE_GRAPHICS_WINDOW_MODE_H_

typedef enum {
  WINDOWED = 0,    // Normal resizable window
  FULLSCREEN = 1,  // True fullscreen mode
  BORDERLESS = 2,  // Borderless fullscreen window
  MAXIMIZED = 3    // Maximized window
} window_mode_t;

#endif  // ENGINE_CORE_GRAPHICS_WINDOW_MODE_H_
