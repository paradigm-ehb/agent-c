#include "base_arena.h"
#include "base_parse.h"

#include "base.c"
#include "base_arena.c"

#include <fcntl.h>

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

  local_internal char*
parse_proc_files(char *path, char *delim, global_arena *arena)
{
  if (!path || !delim || !arena)
  {
    test(0);
    return NULL;
  }


  umm delim_len = sizeof(*delim);
  int fd = open(path, O_RDONLY);

  /* check if the return is anything other than 0*/
  check(fd);

  u64 bytes = 0;

  char buffer[BUFFER_SIZE_SMALL];

  /* processor   : 0 */

  while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
  {
    char *start;
    char *end;

    for ( u64 buffer_position = 1;
        buffer_position < bytes;
        ++buffer_position)
    {

      char *line = &buffer[buffer_position];

      for  (u64 line_position = 1;
          line_position < bytes;
          ++line_position)
      {

        if (line[line_position] == delim[0])
        {

          start = &line[line_position];
          /*
           * TODO(nasr): calculate by end - current
           *
           * */

        }

      }

    }

  }
}

int main(int argc, char **argv)
{
  global_arena *arena = arena_create(MiB(8));

  char *path = "/proc/cpuinfo";
  char *out  = parse_proc_files(path, ":", arena);


  return 0;
}
