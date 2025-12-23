
//*
 * name: Abdellah El Morabit
 * organization: Paradigm-Ehb
 * year: 2025-2026
 * description: Test program for agent_resources library
 */

#include "agent_resources.h"
#include <stdio.h>
#include <stdlib.h>

 //------------------------------------------------------------
#define ARENA_SIZE (1024 * 1024 * 10) // 10MB arena

void
print_separator(const char *title)
 {
   printf("\n=== %s ===\n", title);
 }

 int
 main(void)
 {
   // Create memory arena
   mem_arena *arena = arena_create(ARENA_SIZE);
   if (!arena)
   {
     fprintf(stderr, "Failed to create arena\n");
     return 1;
   }

   printf("Memory Arena Test Program\n");
   printf("Arena capacity: %lu bytes\n", arena->capacity);

   // Test CPU information
   print_separator("CPU Information");
   Cpu *cpu = cpu_create(arena);
   if (!cpu)
   {
     fprintf(stderr, "Failed to create CPU structure\n");
     goto cleanup;
   }

   if (cpu_read_amd64(cpu) == OK)
   {
     printf("Vendor:    %s\n", cpu_get_vendor(cpu));
     printf("Model:     %s\n", cpu_get_model(cpu));
     printf("Frequency: %s MHz\n", cpu_get_frequency(cpu));
     printf("Cores:     %s\n", cpu_get_cores(cpu));
   }
   else
   {
     printf("Failed to read CPU information\n");
   }

   // Test ARM64 CPU cores if available
   int enabled_cores = cpu_get_enabled_cores_arm64();
   if (enabled_cores > 0)
   {
     printf("ARM64 enabled cores: %d\n", enabled_cores);
   }

   // Test RAM information
   print_separator("RAM Information");
   Ram *ram = ram_create(arena);
   if (!ram)
   {
     fprintf(stderr, "Failed to create RAM structure\n");
     goto cleanup;
   }

   if (ram_read(ram) == OK)
   {
     printf("Total: %s KB\n", ram_get_total(ram));
     printf("Free:  %s KB\n", ram_get_free(ram));
   }
   else
   {
     printf("Failed to read RAM information\n");
   }

   // Test Disk information
   print_separator("Disk Partitions");
   Disk *disk = disk_create(arena);
   if (!disk)
   {
     fprintf(stderr, "Failed to create Disk structure\n");
     goto cleanup;
   }

   if (disk_read(disk, arena) == OK)
   {
     size_t count = disk_get_count(disk);
     printf("Found %zu partition(s)\n\n", count);

     for (size_t i = 0; i < count; i++)
     {
       Partition *p = &disk->parts[i];
       printf("Partition %zu:\n", i + 1);
       printf("  Name:   %s\n", partition_get_name(p));
       printf("  Major:  %lu\n", partition_get_major(p));
       printf("  Minor:  %lu\n", partition_get_minor(p));
       printf("  Blocks: %lu\n", partition_get_blocks(p));
       printf("\n");
     }
   }
   else
   {
     printf("Failed to read disk information\n");
   }

   // Test Device information
   print_separator("Device Information");
   Device *device = device_create(arena);
   if (!device)
   {
     fprintf(stderr, "Failed to create Device structure\n");
     goto cleanup;
   }

   if (device_read(device) == OK)
   {
     printf("OS Version: %s", device_get_os_version(device));
     printf("Uptime:     %s", device_get_uptime(device));
   }
   else
   {
     printf("Failed to read device information\n");
   }

   // Test Process collection
   print_separator("Running Processes");
   collect_processes(device, arena);
   size_t proc_count = device_get_procs_count(device);
   printf("Found %zu running processes\n", proc_count);

   if (proc_count > 0)
   {
     printf("\nFirst 10 process PIDs:\n");
     size_t display_count = proc_count < 10 ? proc_count : 10;
     char **procs = device_get_procs(device);

     for (size_t i = 0; i < display_count; i++)
     {
       printf("  PID: %s\n", procs[i]);
     }

     if (proc_count > 10)
     {
       printf("  ... and %zu more\n", proc_count - 10);
     }
   }

   // Test Process stats for first process
   if (proc_count > 0)
   {
     print_separator("Process Stats Sample");
     Proces *proc_stats = arena_push(arena, sizeof(Proces), 1);
     if (proc_stats)
     {
       char **procs = device_get_procs(device);
       if (collect_processes_stats(procs[0], proc_stats, arena) == OK)
       {
         printf("Successfully collected stats for PID: %s\n", proc_stats->pid);
       }
       else
       {
         printf("Failed to collect process stats\n");
       }
     }
   }

   // Arena statistics
   print_separator("Memory Arena Statistics");
   printf("Arena capacity: %lu bytes\n", arena->capacity);
   printf("Arena used:     %lu bytes\n", arena->pos);
   printf("Arena free:     %lu bytes\n", arena->capacity - arena->pos);
   printf("Usage:          %.2f%%\n",
     (double)arena->pos / arena->capacity * 100.0);

   printf("\n=== Test Complete ===\n");

 cleanup:
   arena_destroy(arena);
   return 0;
 }
