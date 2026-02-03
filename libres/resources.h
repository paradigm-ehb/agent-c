#ifndef RESOURCES_H
#define RESOURCES_H
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/statfs.h>

#include "base/base.h"
#include "base/base_arena.h"

#define RESOURCES_API_VERSION 1

typedef struct sys_cpu          sys_cpu;
typedef struct sys_memory       sys_memory;
typedef struct sys_disk         sys_disk;
typedef struct sys_device       sys_device;
typedef struct sys_filesystem   sys_fs;
typedef struct sys_partition    sys_partition;
typedef struct sys_process      sys_proc;
typedef struct sys_process_list sys_proc_list;

typedef enum
{
    proc_undefined       = 0,
    proc_running         = 1,
    proc_sleeping        = 2,
    proc_disk_sleeping   = 3,
    proc_stopped         = 4,
    proc_tracing_stopped = 5,
    proc_zombie          = 6,
    proc_dead            = 7,
    proc_idle            = 8,

} sys_proc_state;

struct sys_process
{
    char           name[16];
    i8             num_threads;
    u64            stime;
    u64            utime;
    i32            pid;
    sys_proc_state state;
};

struct sys_process_list
{
    sys_proc *items;
    umm       count;
    umm       capacity;
};

struct sys_partition
{
    u64 blocks;
    u32 major;
    u32 minor;

    char name[32];
};

struct sys_cpu
{
    u64 total_time;
    u64 idle_time;

    u32 cores;
    u32 _pad;

    char vendor[16];
    char model[64];
    char frequency[16];
};

struct sys_memory
{
    u64 total;
    u64 free;
};

struct sys_filesystem
{
    u64 total;
    u64 free;
    u64 available;
    u64 used;
};

struct sys_disk
{
    sys_partition *partitions;

    umm part_count;
    umm part_capacity;
};

/*
 * TODO(nasr): clean up this ugly struct
 * takes 544 bytes (8 byte alignment)
 * */
struct sys_device
{
    char          os_version[BUFF_DEFAULT];
    char          uptime[BUFF_DEFAULT];
    sys_proc_list processes;
    struct Port  *port;
};

internal sys_cpu *
cpu_create(mem_arena *arena);

internal int
cpu_read(sys_cpu *out);

internal int
cpu_read_usage(sys_cpu *out);

internal int
cpu_get_cores_enabled_arm(sys_cpu *out);

internal int
cpu_read_cpu_model_name_arm64(sys_cpu *out);

internal int
cpu_read_enabled_core_cpu_frequency(sys_cpu *out, int enabled_cpu_count);

internal sys_memory *
ram_create(mem_arena *arena);

internal int
ram_read(sys_memory *out);

internal sys_disk *
disk_create(mem_arena *arena);

internal int
disk_read(sys_disk *out, mem_arena *arena);

internal sys_fs *
fs_create(mem_arena *arena);

internal int
fs_read(char *path, sys_fs *fs);

internal sys_device *
device_create(mem_arena *arena);

internal int
device_read(sys_device *out);

internal int
device_up_time(sys_device *out);

internal int
process_list_collect(sys_proc_list *list, mem_arena *arena);

internal int
process_read(int32_t pid, sys_proc *out);

internal int
process_kill(pid_t pid, int signal);

#endif /* RESOURCES_H */
