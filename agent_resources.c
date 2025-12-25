/*
 * name: Abdellah El Morabit
 * organization: Paradigm-Ehb
 * year: 2025-2026
 * description: resources gathering library
 *
 */

#define _POSIX_C_SOURCE 200809L

#include "agent_resources.h"

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Arena Helper macro's
 * */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ALIGN_UP_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))

/*
 * Represents a disk partition with major/minor device numbers and block count.
 */

/**
 * replacing malloc/eree with arena allocaters
 *
 * */

#define ARENA_BASE_POS (sizeof(mem_arena))
// void * for the size of a pointer on the machine, 64/32bit comp
#define ARENA_ALIGN (sizeof(void *))

struct mem_arena
{
  u64 capacity;
  u64 pos;
};

// arena prototypes
mem_arena *
arena_create(u64 capacity);
// make it a void pointer to allow implicit conversion
void
arena_destroy(mem_arena *arena);
void *
arena_push(mem_arena *arena, u64 size, b32 non_zero);
void
arena_pop(mem_arena *arena, u64 size);
void
arena_pop_to(mem_arena *arena, u64 pos);
void
arena_clear(mem_arena *arena);

mem_arena *
arena_create(u64 capacity)
{
  mem_arena *arena = mmap(0, capacity, PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (arena == MAP_FAILED)
  {
    assert(0);
  }

  arena->capacity = capacity;
  arena->pos = ARENA_BASE_POS;

  return arena;
}

// make it a void pointer to allow implicit conversion
void
arena_destroy(mem_arena *arena)
{
  munmap(arena, arena->capacity);
}

void *
arena_push(mem_arena *arena, u64 size, b32 non_zero)
{
  u64 pos_aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALIGN);
  u64 new_pos = pos_aligned + size;

  if (new_pos > arena->capacity)
  {
    assert(0);
    return NULL;
  }

  arena->pos = new_pos;
  // cast to u8 to be able to do pointer arithemtic
  u8 *out = (u8 *)arena + pos_aligned;

  if (!non_zero)
  {
    memset(out, 0, size);
  }
  return out;
}
void
arena_pop(mem_arena *arena, u64 size)
{
  size = MIN(size, arena->pos - ARENA_BASE_POS);
  arena->pos -= size;
}

void
arena_pop_to(mem_arena *arena, u64 pos)
{
  u64 size = pos < arena->pos ? arena->pos - pos : 0;
  arena_pop(arena, size);
}

void
arena_clear(mem_arena *arena)
{
  arena_pop_to(arena, ARENA_BASE_POS);
}

#define PUSH_STRUCT(arena, T) (T *)arena_push((arena), sizeof(T), 0)
#define PUSH_STRUCT_NZ(arena, T) (T *)arena_push((arena), sizeof(T), 1)
#define PUSH_ARRAY(arena, T, n) (T *)arena_push((arena), sizeof(T) * (n), 0)
#define PUSH_ARRAY_NZ(arena, T, n) (T *)arena_push((arena), sizeof(T) * (n), 1)

/*
 * disk_push_partition - Add a partition to the disk structure
 * @d: Pointer to the Disk structure
 * @p: Partition to add
 *
 * Dynamically grows the partition array if needed. Doubles capacity when
 * full. If realloc fails, the partition is not added and function returns
 * silently.
 */

void
disk_push_partition(Disk *d, Partition p, mem_arena *arena)
{
  if (d->count == d->capacity)
  {
    size_t new_cap = d->capacity ? d->capacity * 2 : 8;

    Partition *np = PUSH_ARRAY_NZ(arena, Partition, new_cap);
    if (!np)
      return;

    if (d->partitions && d->count > 0)
    {
      memcpy(np, d->partitions, d->count * sizeof(Partition));
    }

    d->partitions = np;
    d->capacity = new_cap;
  }

  d->partitions[d->count++] = p;
}

/*
 * is_numeric - Check if a string contains only digits
 * @s: String to check
 *
 * Return: 1 if string contains only numeric characters, 0 otherwise
 */
int
is_numeric(const char *s)
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

/*
 * cpu_create - Allocate and initialize a new Cpu structure
 *
 * Return: Pointer to newly allocated Cpu, or NULL on allocation failure
 */
Cpu *
cpu_create(mem_arena *m)
{
  return arena_push(m, sizeof(Cpu), 1);
}

/*
 * cpu_read - Read CPU information from /proc/cpuinfo
 * @out: Pointer to Cpu structure to populate
 *
 * Reads vendor_id, model name, cpu MHz, and cpu cores from /proc/cpuinfo.
 * The function reads the first occurrence of each field.
 *
 * Return: OK on success, AGENT_ERR_INVALID if out is NULL,
 *         ERR_IO if /proc/cpuinfo cannot be opened
 */

/**
 * get a read of the enabled cpu cores to get a view of the folder structure
 * before starting to read every single of of them
 *
 * call
 *
 * */
int
cpu_read_enabled_core_cpu_frequency(Cpu *out, int enabled_cpu_count)
{
  if (!out)
  {
    return ERR_INVALID;
  }

  char curr_freq[BUFFER_SIZE_SMALL];
  char path[PATH_MAX_LEN];

  snprintf(path, sizeof(path), "/sys/devices/system/cpu/cpu%d/cpuinfo_cur_freq",
    enabled_cpu_count);

  FILE *fp = fopen(path, "r");
  if (!fgets(curr_freq, sizeof(curr_freq), fp))
  {
    return ERR_PARSE;
  }

  size_t len = sizeof(curr_freq) / sizeof(char);
  memcpy(out->frequency, curr_freq, len);
  out->frequency[len] = '\0';
  // AGAIN!!! DONT FORGET TO NULL TERMINATE STRINGS

  fclose(fp);

  return OK;
}
// TODO(nasr): i was doing something with directories i forgot what

// TODO(nasr): read the binary /proc/device-tree/model
// it contains the device model + cpu model
int
cpu_read_cpu_model_name_arm64(Cpu *out)
{
  int of = open("/proc/device-tree/model", O_RDONLY);

  u8 buffer[BUFFER_SIZE_DEFAULT];
  size_t s = sizeof(buffer);
  ssize_t rf = read(of, buffer, s);

  if (rf <= 0)
  {
    printf("yay warning gone grr");
  }

  printf("%s", out->frequency);

  return OK;
}

int
cpu_get_enabled_cores_arm64(void)
{
  FILE *fp = fopen("/sys/devices/system/cpu/enabled", "r");
  if (!fp)
  {
    return -1;
  }

  char buffer[BUFFER_SIZE_DEFAULT];
  if (!fgets(buffer, sizeof(buffer), fp))
  {
    fclose(fp);
    return -1;
  }
  fclose(fp);

  int max_core = 0;
  int dash = 0;

  for (size_t i = 0; buffer[i] != '\0'; ++i)
  {
    char c = buffer[i];

    if (c == '-')
    {
      dash = 1;
      continue;
    }

    if (!dash)
    {
      continue;
    }

    if (c < '0' || c > '9')
    {
      break;
    }

    max_core = max_core * 10 + (c - '0');
  }

  return max_core + 1;
}

int
cpu_read_arm64(Cpu *out)
{
  if (!out)
  {
    return ERR_INVALID;
  }

  // TODO(nasr): gather all information arm
  return OK;
}

int
cpu_read_amd64(Cpu *out)
{
  if (!out)
  {
    return ERR_INVALID;
  }

  FILE *f = fopen("/proc/cpuinfo", "r");
  if (!f)
    return ERR_IO;

  char buf[BUFFER_SIZE_LARGE];
  while (fgets(buf, sizeof(buf), f))
  {
    char *colon = strchr(buf, ':');
    if (!colon)
      continue;

    char *val = colon + 1;
    while (*val == ' ')
      val++;

    size_t len = strcspn(val, "\n");

    if (!strncmp(buf, "vendor_id", 9))
    {
      memcpy(out->vendor, val, len);
    }
    if (!strncmp(buf, "model name", 10))
    {
      memcpy(out->model, val, len);
    }
    if (!strncmp(buf, "cpu MHz", 7))
    {
      memcpy(out->frequency, val, len);
    }
    if (!strncmp(buf, "cpu cores", 9))
    {
      memcpy(out->cores, val, len);
    }
  }

  fclose(f);
  return OK;
}

int
cpu_read(Cpu *out)
{
  // TODO(nasr): C macro check for architecture and call appropriate function
  printf("%s", out->model);

  return OK;
}

/*
 * ram_create - Allocate and initialize a new Ram structure
 *
 * Return: Pointer to newly allocated Ram, or NULL on allocation failure
 */
Ram *
ram_create(mem_arena *m)
{
  return arena_push(m, sizeof(Ram), 1);
}

/*
 * ram_read - Read RAM information from /proc/meminfo
 * @out: Pointer to Ram structure to populate
 *
 * Reads MemTotal and MemFree from /proc/meminfo in kilobytes.
 *
 * Return: OK on success, AGENT_ERR_INVALID if out is NULL,
 *         ERR_IO if /proc/meminfo cannot be opened
 */
int
ram_read(Ram *out)
{
  if (!out)
    return ERR_INVALID;

  FILE *f = fopen("/proc/meminfo", "r");
  if (!f)
    return ERR_IO;

  char buf[BUFFER_SIZE_LARGE];
  while (fgets(buf, sizeof(buf), f))
  {
    char *colon = strchr(buf, ':');
    if (!colon)
    {
      continue;
    }

    char *val = colon + 1;
    while (*val == ' ')
    {
      val++;
    }

    size_t len = strcspn(val, " k\n");

    if (!strncmp(buf, "MemTotal", 8))
    {
      memcpy(out->total, val, len);
    }
    else if (!strncmp(buf, "MemFree", 7))
    {
      memcpy(out->free, val, len);
    }
  }

  fclose(f);
  return OK;
}

/*
 * disk_create - Allocate and initialize a new Disk structure
 *
 * Return: Pointer to newly allocated Disk, or NULL on allocation failure
 */
Disk *
disk_create(mem_arena *m)
{
  return arena_push(m, sizeof(Disk), 1);
}

/*
 * disk_read - Read disk partition information from /proc/partitions
 * @out: Pointer to Disk structure to populate
 *
 * Reads all partitions from /proc/partitions, storing major/minor device
 * numbers, block count, and device name for each partition. Skips the header
 * line and any malformed entries.
 *
 * Return: OK on success, AGENT_ERR_INVALID if out is NULL,
 *         ERR_IO if /proc/partitions cannot be opened
 */

int
disk_read(Disk *out, mem_arena *arena)
{
  if (!out)
    return ERR_INVALID;

  FILE *f = fopen("/proc/partitions", "r");
  if (!f)
    return ERR_IO;

  char buf[BUFFER_SIZE_DEFAULT];

  fgets(buf, sizeof(buf), f);
  fgets(buf, sizeof(buf), f);

  while (fgets(buf, sizeof(buf), f))
  {
    Partition p = { 0 };
    char name[BUFFER_SIZE_DEFAULT];

    if (sscanf(buf, "%lu %lu %lu %255s", &p.major, &p.minor, &p.blocks, name) != 4)
    {
      continue;
    }

    size_t len = strlen(name);
    if (len >= sizeof(p.name))
      len = sizeof(p.name) - 1;

    memcpy(p.name, name, len);
    p.name[len] = 0;

    disk_push_partition(out, p, arena);
  }

  fclose(f);
  return OK;
}
/*
 * device_create - Allocate and initialize a new Device structure
 *
 * Return: Pointer to newly allocated Device, or NULL on allocation failure
 */
Device *
device_create(mem_arena *m)
{
  return arena_push(m, sizeof(Device), 1);
}

/*
 * collect_processes - Collect all running process IDs from /proc
 * @dev: Pointer to Device structure to populate with process IDs
 *
 * Scans /proc directory for numeric entries (process IDs) and stores them
 * as strings in the Device structure. Dynamically grows the process array
 * as needed.
 */

int
process_list_collect(Process_List *list, mem_arena *arena)
{
  DIR *d = opendir("/proc");
  if (!d)
    return ERR_IO;

  struct dirent *e = 0;

  if (!list->items)
  {
    list->capacity = 8;
    list->count = 0;
    list->items = PUSH_ARRAY_NZ(arena, Process, list->capacity);
    if (!list->items)
    {
      closedir(d);
      return ERR_IO;
    }
  }

  while ((e = readdir(d)))
  {
    if (!is_numeric(e->d_name))
      continue;

    if (list->count == list->capacity)
    {
      size_t new_cap = list->capacity * 2;
      Process *np = PUSH_ARRAY_NZ(arena, Process, new_cap);
      if (!np)
        break;

      memcpy(np, list->items, sizeof(Process) * list->capacity);
      list->items = np;
      list->capacity = new_cap;
    }

    Process *p = &list->items[list->count++];

    p->pid = atoi(e->d_name);
    p->state = PROCESS_UNDEFINED;
    p->utime = 0;
    p->stime = 0;
    p->num_threads = 0;
    p->name[0] = 0;
  }

  closedir(d);
  return OK;
}

/**
 *
struct Proces {
  char *pid;
        char *name;
  char *state;
  char *utime;
  char *num_threads;
};

*/

int
process_read(i32 pid, Process *out)
{
  char path[PATH_MAX_LEN];
  snprintf(path, sizeof(path), "/proc/%d/status", pid);

  FILE *fp = fopen(path, "r");
  if (!fp)
    return ERR_IO;

  char buf[BUFFER_SIZE_LARGE];

  out->pid = pid;
  out->state = PROCESS_UNDEFINED;
  out->utime = 0;
  out->stime = 0;
  out->num_threads = 0;
  out->name[0] = 0;

  while (fgets(buf, sizeof(buf), fp))
  {
    char *colon = strchr(buf, ':');
    if (!colon)
      continue;

    char *val = colon + 1;
    while (*val == ' ' || *val == '\t')
      ++val;

    size_t len = strcspn(val, "\n");

    if (!strncmp(buf, "Name", 4))
    {
      if (len >= sizeof(out->name))
        len = sizeof(out->name) - 1;

      memcpy(out->name, val, len);
      out->name[len] = 0;
    }
    else if (!strncmp(buf, "State", 5))
    {
      switch (val[0])
      {
        case 'R':
        {
          out->state = PROCESS_RUNNING;
          break;
        }
        case 'S':
        {
          out->state = PROCESS_SLEEPING;
          break;
        }
        case 'D':
        {
          out->state = PROCESS_DISK_SLEEP;
          break;
        }
        case 'T':
        {
          out->state = PROCESS_STOPPED;
          break;
        }
        case 't':
        {
          out->state = PROCESS_TRACING_STOPPED;
          break;
        }
        case 'Z':
        {
          out->state = PROCESS_ZOMBIE;
          break;
        }
        case 'X':
        {
          out->state = PROCESS_DEAD;
          break;
        }
        default:
        {
          out->state = PROCESS_UNDEFINED;
          break;
        }
      }
    }
    else if (!strncmp(buf, "Threads", 7))
    {
      out->num_threads = (u32)strtoul(val, 0, 10);
    }
  }

  fclose(fp);
  return OK;
}

int
device_up_time(Device *out)
{
  if (!out)
  {
    return ERR_IO;
  }

  FILE *uptime = fopen("/proc/uptime", "r");
  if (!uptime)
  {
    if (uptime)
    {
      fclose(uptime);
    }

    return ERR_IO;
  }

  fgets(out->uptime, sizeof(out->uptime), uptime);

  fclose(uptime);

  return OK;
}

/*
 * device_read - Read device information including OS version, uptime, and
 * processes
 * @out: Pointer to Device structure to populate
 *
 * Reads OS version from /proc/version, uptime from /proc/uptime, and collects
 * all running process IDs from /proc directory.
 *
 * Return: OK on success, AGENT_ERR_INVALID if out is NULL,
 *         ERR_IO if required files cannot be opened
 */
int
device_read(Device *out)
{
  if (!out)
  {
    return ERR_INVALID;
  }

  FILE *version = fopen("/proc/version", "r");
  if (!version)
  {
    if (version)
    {
      fclose(version);
    }
    return ERR_IO;
  }

  fgets(out->os_version, sizeof(out->os_version), version);

  fclose(version);

  return OK;
}

/*
 * process_kill - Send a signal to a process
 * @pid: Process ID to signal
 * @signal: Signal number to send (e.g., SIGTERM, SIGKILL)
 *
 * Wrapper around kill(2) system call with error handling.
 *
 * Return: OK on success,
 *         ERR_INVALID if pid is invalid or process not found,
 *         ERR_PERM if permission denied,
 *         ERR_IO for other errors
 */
int
process_kill(pid_t pid, int signal)
{
  if (pid <= 0)
  {
    return ERR_INVALID;
  }

  if (kill(pid, signal) == -1)
  {
    if (errno == EPERM)
    {
      return ERR_PERM;
    }
    if (errno == ESRCH)
    {
      return ERR_INVALID;
    }

    return ERR_IO;
  }
  return OK;
}
