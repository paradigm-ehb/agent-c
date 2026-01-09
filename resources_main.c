#include "base.h"
#include "resources.h"
#include "stdio.h"
#include "arena.h"
#include "arena.c"
#include "resources.c"
#include <assert.h>


int main()
{

  mem_arena *arena = arena_create(MiB(1));

  Device *device =  device_create(arena);
  process_list_collect(&device->processes, arena);

  for (size_t proc_idx;
    proc_idx < device->processes.count;
    ++proc_idx)
    {

      Process *proc = (Process*)arena_push(arena,
        sizeof(Process),
        1);

      printf("\nstate: %d\n", proc->state);

      i32 error = process_read(device->processes
      
      .items[proc_idx]
      .pid,
      proc);

      printf("\nstate: %d\n", proc->state);

      if (error != OK)
      {

        assert(0);
      }

    printf("after %d\n", proc->state);
    }


  return 0;
}
