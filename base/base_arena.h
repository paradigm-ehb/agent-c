#ifndef ARENA_H
#define ARENA_H

#include "base.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * Arena Helper macro's
 * */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ALIGN_UP_POW2(n, p) align_up_pow((u64)(n), (u64)(p))
#define PUSH_STRUCT(arena, T) (T *)arena_alloc((arena), sizeof(T))
#define PUSH_ARRAY(arena, T, n) (T *)arena_alloc((arena), sizeof(T) * (n))
#define ARENA_ALIGN (sizeof(void *))

typedef struct mem_arena  mem_arena;
typedef struct temp_arena temp_arena;

struct mem_arena
{
    u64 capacity;
    u64 pos;
    u64 prev_pos;
};

struct temp_arena
{
    mem_arena *arena;
    umm        offset;
    umm        prev_offset;
};

local_internal inline u64
align_up_pow(u64 n, u64 p);

local_internal mem_arena *
arena_create(u64 capacity);
local_internal void
arena_destroy(mem_arena *arena);
local_internal void *
arena_alloc(mem_arena *arena, u64 size);
local_internal void
arena_pop(mem_arena *arena, u64 size);
local_internal void
arena_pop_to(mem_arena *arena, u64 pos);
local_internal void
arena_clear(mem_arena *arena);

local_internal void
temp_arena_create(mem_arena *arena, u64 size);
local_internal void
temp_arena_destroy(mem_arena *arena, u64 size);

#endif
