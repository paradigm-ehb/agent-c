#include "base/base.h"
#include <cstdio>

#ifdef DEBUG

/**
 * TODO(nasr): implement this posix flag
 * */
#define FLAGS
#elif defined(RELEASE)
#define FLAGS
#endif

local_internal void
compile(char *path)
{
  /**
   * TODO(nasr): call clang somehow
   * */
  std::printf("path %s", path);
}

local_internal int
build(int argc, char **argv)
{
  if (argc > 0)
  {
    printf("arguments provided");
  }

  compile(*argv);
  return 0;
}
