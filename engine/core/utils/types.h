#ifndef ENGINE_CORE_UTILS_TYPES_H_
#define ENGINE_CORE_UTILS_TYPES_H_

#include <stdint.h>

/**
 * Common type aliases for better type safety and code clarity.
 *
 * These types make intent explicit and help catch mistakes at compile time.
 */

// Time-related types
typedef uint32_t timestamp_ms_t;  // Timestamp in milliseconds

// Rotation/angle types (0-63 for ship rotation indices)
typedef uint8_t rotation_index_t;

// Entity scale types
typedef enum {
  ENTITY_SCALE_SMALL = 2,
  ENTITY_SCALE_MEDIUM = 3,
  ENTITY_SCALE_LARGE = 6
} entity_scale_t;

#endif  // ENGINE_CORE_UTILS_TYPES_H_
