#ifndef BAsE_PARSE_H
#define BAsE_PARSE_H

#include "base/base_arena.h"
#include <base/base.h>
#include <fcntl.h>

typedef struct
{
    char *key;
    char *value[];
} ProcEntry;

typedef struct
{
    i8 *start;
    i8 *end;
    umm len;
} Line;

local_internal b8
compare_string(const char *c1, const char *c2);

local_internal ProcEntry *
parse_proc_files(const char *path, mem_arena *arena);

local_internal u64
parse_u64(char *buf, size_t len);

local_internal b8
is_numeric(char *s);

#endif
