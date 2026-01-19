#include "base/base.h"
#include "base/base_arena.h"
#include "libres/resources.h"

#include "base/base.c"
#include "base/base_arena.c"
#include "libres/resources.cc"

#include <assert.h>
#include <stdio.h>

local_internal void
disk_tests()
{
  mem_arena *arena = arena_create(MiB(8));

  Disk *disk = disk_create(arena);
  disk_read(disk, arena);

  printf("disk values %lu", disk->part_count);
}

int
main()
{
  disk_tests();
}
