/**
 * @file object_pool.h
 * @brief Generic object pool for efficient memory management
 *
 * Provides a reusable object pool implementation with constant-time
 * allocation and deallocation. Uses contiguous memory for cache efficiency
 * and maintains a free list for fast object reuse without heap operations.
 */

#ifndef ENGINE_CORE_MEMORY_OBJECT_POOL_H_
#define ENGINE_CORE_MEMORY_OBJECT_POOL_H_

#include <stdbool.h>
#include <stdlib.h>

// Generic object pool for efficient memory management
// Provides constant-time allocation and deallocation
typedef struct {
  void* objects;           // Contiguous array of objects
  size_t object_size;      // Size of each object in bytes
  size_t capacity;         // Maximum number of objects
  size_t* free_indices;    // Stack of available indices
  size_t free_count;       // Number of free slots
  bool* active_flags;      // Which slots are currently active
  size_t active_count;     // Number of active objects
} object_pool_t;

// Create an object pool with specified object size and capacity
// Returns initialized pool (caller must call pool_destroy when done)
object_pool_t create_object_pool(size_t object_size, size_t capacity);

// Acquire an object from the pool
// Returns pointer to object and sets out_index to its index
// Returns NULL if pool is full
void* pool_acquire(object_pool_t* pool, size_t* out_index);

// Release an object back to the pool by index
void pool_release(object_pool_t* pool, size_t index);

// Reset pool to empty state (marks all objects as inactive)
void pool_reset(object_pool_t* pool);

// Get number of active objects in pool
size_t pool_get_active_count(const object_pool_t* pool);

// Get object at specific index (does not check if active)
void* pool_get_at(object_pool_t* pool, size_t index);

// Check if object at index is active
bool pool_is_active(const object_pool_t* pool, size_t index);

// Destroy pool and free all memory
void pool_destroy(object_pool_t* pool);

// Callback type for iteration
typedef void (*pool_callback_t)(void* object, size_t index, void* user_data);

// Iterate over all active objects
void pool_foreach_active(object_pool_t* pool, pool_callback_t callback,
                         void* user_data);

#endif  // ENGINE_CORE_MEMORY_OBJECT_POOL_H_
