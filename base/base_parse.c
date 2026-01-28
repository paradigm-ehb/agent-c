#include "base/base_parse.h"
#include "base/base.h"
#include "base/base_arena.h"

local_internal b8
compare_string(const char *c1, const char *c2)
{
    if (sizeof(c1) != sizeof(c2))
    {
        return -1;
    }

    for (
    u64 word_idx = 0;
    word_idx <= sizeof(*c1);
    ++word_idx)
    {
        if (*c1 != *c2)
        {
            return -1;
        }
        ++c1;
        ++c2;
    }

    return 0;
}

/**
 * Helper function to parse strings to int using ascii codes
 * */
local_internal u64
parse_u64(char *buf, umm len)
{
    u64 value = 0;

    for (
    umm buffer_idx = 0;
    buffer_idx < len;
    ++buffer_idx)
    {
        char c = buf[buffer_idx];
        if (c < '0' || c > '9')
        {
            break;
        }
        value = value * 10 + (c - '0');
    }

    return value;
}

local_internal ProcEntry *
parse_proc_files(const char *path, mem_arena *arena)
{
    local_persist const char KEY_DELIM      = ':';
    local_persist const char VALUE_DELIMS[] = {
    ' ',
    '\t',
    };
    local_persist const char RECORD_DELIM = '\n';

    ProcEntry *entry = PUSH_STRUCT(arena, ProcEntry);
    if (!path || !arena)
    {
        test(0);
        return NULL;
    }

    i32   fd;
    u64   bytes;
    char *buffer[BUFF_SMALL];

    bytes = read(fd, buffer, sizeof(buffer));
    {
        char *start;
        char *end;

        char buffer[BUFF_DEFAULT];

        /* iteration over the buffer to split it up in lines */
        for (u64 buffer_position = 0;
        buffer_position < bytes;
        ++buffer_position)
        {
            /* define line */
            if (buffer[buffer_position] == RECORD_DELIM)
            {
            }

            for (u64 line_position = 0;
            line_position < bytes;
            ++line_position)
            {
                char *line_bf = &buffer[buffer_position];

                if (line_bf[line_position] == '\t' || line_bf[line_position] == ' ')
                {
                    continue;
                }

                if (line_bf[line_position] == KEY_DELIM)
                {
                    start = &line_bf[line_position];
                    continue;
                }

                start = &line_bf[line_position];
                if (line_bf[line_position] == '\n')
                {
                    end = &line_bf[line_position];
                    break;
                }
            }
            // break;
        }

        close(fd);
        return entry;
    }
}

/*
 * is_numeric - Check if a string contains only digits
 * @s: String to check
 *
 * Return: 1 if string contains only numeric characters, 0 otherwise
 */
local_internal b8
is_numeric(char *s)
{
    for (; *s; ++s)
    {
        if (*s < '0' || *s > '9')
        {
            return 0;
        }
    }
    return 1;
}

#ifdef DEBUG
int
main(void)
{
    mem_arena *arena = arena_create(MiB(8));

    const char *path  = "/proc/cpuinfo";
    const char *delim = ":";

    parse_proc_files(path, arena);

    return 0;
}
#endif
