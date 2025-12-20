#ifndef AGENT_RESOURCES_H
#define AGENT_RESOURCES_H

#include <sys/types.h>

#define AGENT_RESOURCES_API_VERSION 1

typedef enum {
  AGENT_OK = 0,
  AGENT_ERR_IO,
  AGENT_ERR_PARSE,
  AGENT_ERR_PERM,
  AGENT_ERR_INVALID,
} AgentResult;

/**
 * These structs are intentionally incomplete here.
 * Their definitions live in the .c file.
 *
 */

typedef struct AgentCpu AgentCpu;
typedef struct AgentRam AgentRam;
typedef struct AgentDisk AgentDisk;
typedef struct AgentDevice AgentDevice;

AgentCpu *agent_cpu_create(void);
AgentRam *agent_ram_create(void);
AgentDisk *agent_disk_create(void);
AgentDevice *agent_device_create(void);

void agent_cpu_destroy(AgentCpu *cpu);
void agent_ram_destroy(AgentRam *ram);
void agent_disk_destroy(AgentDisk *disk);
void agent_device_destroy(AgentDevice *device);

AgentResult agent_cpu_read(AgentCpu *cpu);
AgentResult agent_ram_read(AgentRam *ram);
AgentResult agent_disk_read(AgentDisk *disk);
AgentResult agent_device_read(AgentDevice *device);

AgentResult agent_process_kill(pid_t pid, int signal);

#endif
