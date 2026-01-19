#include "base/base.h"
#include "base/base_arena.h"
#include "libres/resources.h"

#include "base/base.c"
#include "base/base_arena.c"
#include "libres/resources.cc"

#include <assert.h>


/* Test case:
 * test if the retrieved disks aren't 0
 * */
local_internal void
test_cpu_name()
{
  mem_arena *arena = arena_create(MiB(8));
  Cpu *cpu = cpu_create(arena);

  cpu_read(cpu);

  assert((cpu->cores != 0) && "Failed to retrieve partition count");
}

int
main()
{
  test_cpu_name();
}
