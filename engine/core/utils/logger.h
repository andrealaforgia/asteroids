/**
 * @file logger.h
 * @brief Logging macros for error and info messages
 *
 * Provides standardized logging macros wrapping SDL logging functions.
 * Includes helpers for SDL errors, SDL_mixer errors, and formatted messages
 * with consistent categorization across the codebase.
 */

#ifndef ENGINE_CORE_UTILS_LOGGER_H_
#define ENGINE_CORE_UTILS_LOGGER_H_

#include <SDL.h>
#include <SDL_mixer.h>

// Error logging macros for consistent error handling across the codebase
// Use these macros for all error messages after SDL initialization

// Log a simple error message
#define LOG_ERROR(msg) SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", msg)

// Log an error with SDL_GetError() details
#define LOG_SDL_ERROR(operation)                                  \
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s Error: %s", operation, \
               SDL_GetError())

// Log an error with Mix_GetError() details (for SDL_mixer errors)
#define LOG_MIX_ERROR(operation)                                  \
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s Error: %s", operation, \
               Mix_GetError())

// Log an informational message
#define LOG_INFO(msg) SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", msg)

// Log a formatted message
#define LOG_INFO_FMT(fmt, ...) \
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)

// Log a warning
#define LOG_WARN(msg) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", msg)

// Log a formatted warning
#define LOG_WARN_FMT(fmt, ...) \
  SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)

#endif  // ENGINE_CORE_UTILS_LOGGER_H_
