#include "object_pool.h"

#include <stdlib.h>
#include <string.h>

object_pool_t create_object_pool(size_t object_size, size_t capacity) {
  object_pool_t pool;
  pool.object_size = object_size;
  pool.capacity = capacity;
  pool.active_count = 0;
  pool.free_count = capacity;

  // Allocate contiguous memory for all objects
  pool.objects = malloc(object_size * capacity);

  // Allocate free indices stack
  pool.free_indices = malloc(sizeof(size_t) * capacity);

  // Allocate active flags
  pool.active_flags = malloc(sizeof(bool) * capacity);

  // Initialize all objects as inactive and add to free list
  for (size_t i = 0; i < capacity; i++) {
    pool.active_flags[i] = false;
    pool.free_indices[i] = capacity - 1 - i;  // Stack: push in reverse order
  }

  return pool;
}

void* pool_acquire(object_pool_t* pool, size_t* out_index) {
  // Check if pool is full
  if (pool->free_count == 0) {
    return NULL;
  }

  // Pop index from free list
  size_t index = pool->free_indices[pool->free_count - 1];
  pool->free_count--;

  // Mark as active
  pool->active_flags[index] = true;
  pool->active_count++;

  // Calculate pointer to object
  void* object = (char*)pool->objects + (index * pool->object_size);

  // Zero out the memory for clean state
  memset(object, 0, pool->object_size);

  if (out_index != NULL) {
    *out_index = index;
  }

  return object;
}

void pool_release(object_pool_t* pool, size_t index) {
  // Validate index
  if (index >= pool->capacity) {
    return;
  }

  // Check if already inactive
  if (!pool->active_flags[index]) {
    return;
  }

  // Mark as inactive
  pool->active_flags[index] = false;
  pool->active_count--;

  // Push index back to free list
  pool->free_indices[pool->free_count] = index;
  pool->free_count++;
}

void pool_reset(object_pool_t* pool) {
  pool->active_count = 0;
  pool->free_count = pool->capacity;

  // Reset all flags and rebuild free list
  for (size_t i = 0; i < pool->capacity; i++) {
    pool->active_flags[i] = false;
    pool->free_indices[i] = pool->capacity - 1 - i;
  }
}

size_t pool_get_active_count(const object_pool_t* pool) {
  return pool->active_count;
}

void* pool_get_at(object_pool_t* pool, size_t index) {
  if (index >= pool->capacity) {
    return NULL;
  }
  return (char*)pool->objects + (index * pool->object_size);
}

bool pool_is_active(const object_pool_t* pool, size_t index) {
  if (index >= pool->capacity) {
    return false;
  }
  return pool->active_flags[index];
}

void pool_destroy(object_pool_t* pool) {
  free(pool->objects);
  free(pool->free_indices);
  free(pool->active_flags);

  // Zero out the struct to prevent use-after-free
  pool->objects = NULL;
  pool->free_indices = NULL;
  pool->active_flags = NULL;
  pool->capacity = 0;
  pool->active_count = 0;
  pool->free_count = 0;
}

void pool_foreach_active(object_pool_t* pool, pool_callback_t callback,
                         void* user_data) {
  for (size_t i = 0; i < pool->capacity; i++) {
    if (pool->active_flags[i]) {
      void* object = pool_get_at(pool, i);
      callback(object, i, user_data);
    }
  }
}
