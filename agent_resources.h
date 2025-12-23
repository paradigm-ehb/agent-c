
#ifndef RESOURCES_H
#define RESOURCES_H

#include <stddef.h>
#include <stdint.h>

#define RESOURCES_API_VERSION 1

#define OK 0
#define ERR_IO 1
#define ERR_PARSE 2
#define ERR_PERM 3
#define ERR_INVALID 4

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef i16 b16;
typedef i32 b32;

/**
 * Mem sizes for Arena Allocation
 * */
static inline u64 KiB(u64 n) { return n << 10; }

static inline u64 MiB(u64 n) { return n << 20; }

static inline u64 GiB(u64 n) { return n << 30; }

typedef struct mem_arena mem_arena;

typedef struct Cpu Cpu;
typedef struct Ram Ram;
typedef struct Disk Disk;
typedef struct Device Device;
typedef struct Proces Proces;

mem_arena *arena_create(uint64_t capacity);
void arena_destroy(mem_arena *arena);
void arena_clear(mem_arena *arena);

Cpu *cpu_create(mem_arena *arena);
Ram *ram_create(mem_arena *arena);
Disk *disk_create(mem_arena *arena);
Device *device_create(mem_arena *arena);

int cpu_read(Cpu *cpu);
int ram_read(Ram *ram);
int disk_read(Disk *disk, mem_arena *m);
int device_read(Device *device);

char *cpu_get_vendor(Cpu *cpu);
char *cpu_get_model(Cpu *cpu);
char *cpu_get_frequency(Cpu *cpu);
char *cpu_get_cores(Cpu *cpu);

char *ram_get_total(Ram *ram);
char *ram_get_free(Ram *ram);

size_t disk_get_count(Disk *disk);
char *disk_get_partitions(Disk *disk);

char *device_get_os_version(Device *device);
char *device_get_uptime(Device *device);
char **device_get_procs(Device *device);
size_t device_get_procs_count(Device *device);

void collect_processes(Device *dev, mem_arena *m);

int process_kill(int pid, int signal);

#endif
