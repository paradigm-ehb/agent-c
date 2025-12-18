#ifndef AGENT_RESOURCES_H
#define AGENT_RESOURCES_H

typedef struct AgentCpu		AgentCpu;
typedef struct AgentRam		AgentRam;
typedef struct AgentDisk	AgentDisk;
typedef struct AgentDevice	AgentDevice;


void
agent_cpu_read(Cpu *cpu);

void
agent_ram_read(Ram *ram);

void 
agent_disk_read(Disk *disk);

void
agent_device_read(Device *device);

int
agent_terminate_proc(int pid);


