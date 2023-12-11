#pragma once

#include "rogue/rl_common.h"

#define RL_ARENA_ALIGN 16U

typedef struct rl_arena_t
{
    void * base;
    char * head;
    char const * end;

} rl_arena_t;

RL_INLINE size_t rl_arena_in_use_memory(rl_arena_t const * arena)
{
    uintptr_t base = arena->base;
    uintptr_t top = arena->head;
    return top - base;
}

RL_INLINE size_t rl_arena_total_capacity(rl_arena_t const * arena)
{
    uintptr_t base = arena->base;
    uintptr_t end = arena->end;
    return end - base;
}

RL_INLINE void rl_arena_initialize(rl_arena_t * arena, void * buffer, size_t buffer_size)
{
    RL_ASSERT(buffer_size > 0);
    RL_ASSERT(buffer != NULL);
    RL_ASSERT(arena != NULL);

    arena->base = buffer;
    arena->head = buffer;
    arena->end = arena->head + buffer_size;

    memset(arena->base, 0, buffer_size);
}

static inline void * rl_arena_allocate(rl_arena_t * arena, size_t requested_size)
{
    RL_ASSERT(requested_size > 0);

    uintptr_t current_address = arena->head;
    uintptr_t offset_from_align = (RL_ARENA_ALIGN - (current_address % RL_ARENA_ALIGN)) % RL_ARENA_ALIGN;
    uintptr_t aligned_address = current_address + offset_from_align;

    uintptr_t end_address = arena->end;

    if (aligned_address + requested_size < end_address)
    {
        char const * allocation = arena->head + offset_from_align;
        arena->head = arena->head + (offset_from_align + requested_size);

        return allocation;
    }
    else
    {
        RL_ASSERT(0);
        return NULL;
    }
}

RL_INLINE void rl_arena_clear(rl_arena_t * arena)
{
    arena->head = arena->base;

    //PERFORMANCE_WARNING(zshoals):> This is a safety measure, we can elide the memset if we just want max performance
    memset(arena->base, 0, rl_arena_total_capacity(arena));
}