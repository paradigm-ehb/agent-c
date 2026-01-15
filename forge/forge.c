#include "base/base.h"

#ifdef DEBUG

/**
 * TODO(nasr): implement this posix flag
 * */
#define FLAGS
#elif defined(RELEASE)
#define FLAGS
#endif

internal void
compile(char *path)
{
  /**
   * TODO(nasr): call clang somehow
   * */
}

int
build(int argc, char **argv)
{
  compile(*argv);
  return 0;
}
