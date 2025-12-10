
/*
 * 
 * Author: nasr
 * Year: 2025-2026
 *
 */

#include <sys/sysinfo.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

#define MAXC 1024
#define MAXC_CHAR 256
#define CONVERT_BYTES_TO_GIGABYTES 107374182   
#define D 1073741824

#define forever for(;;)

typedef struct {

	char* name;
	char* frequency;
	int temperature;
	int threads;

} Cpu;

typedef struct {
	double total;
	double available;
} Ram;

typedef struct {
	long size;
	char *name;
} Disk;

typedef struct { 

	char *name;
	char *hostname;
	char *os_version;
	int uptime;
	uint16_t procs;

} Device;

void cpu_data(Cpu *cpu);
void memory_data(Ram *ram);
void device_data(Device *device);

int 
main() {

	// TODO: get disk information

	Cpu		*cpu		= (Cpu*)malloc( sizeof(Cpu) );
	Ram		*ram		= (Ram*)malloc(sizeof(Ram));
	Device	*device		= (Device*)malloc(sizeof(Device));

	// disk	= (Disk*)malloc(sizeof(Disk));

	cpu_data(cpu);
	memory_data(ram);
	device_data(device);

	printf("gathering system resources...");
	printf("\ntemperature: %d\nfrequency: %s\nname: %s\nthreads: %d\n", cpu->temperature, cpu->frequency, cpu->name, cpu->threads);
	printf("total: %f\navailable: %f\n", ram->total, ram->available);
	printf("device name:%s\ndevice hostname:%s\nos version: %s\nuptime: %d\nprocs: %d\n", device->name, device->hostname, device->os_version, device->uptime, device->procs);

	free(cpu);
	free(ram);
	// free(disk);
	free(device);

	return 0;
}





void
cpu_data(Cpu *cpu) {

	cpu->name	= (char*)malloc(sizeof(char));
	char *buffer = (char*)malloc(sizeof(int));

	FILE *fp = fopen("/proc/cpuinfo", "r");
	if (!fp) perror("can't open /proc/cpuinfo");

	char line[sizeof(cpu->name)];
	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "model",  sizeof(cpu->name)) == 0) {
			char *colon = strchr(line, ':');
			if (colon) {
					cpu->name[strcspn(cpu->name, "\n")] = 0;
					snprintf(cpu->name, MAXC_CHAR, "%s", colon);
			}


			if (strstr(buffer, "cpu MHz") != NULL) {
				colon = NULL;
				colon = strchr(buffer, ':');
				if (colon) {
					buffer[strcspn(buffer, "\n")] = 0;
					snprintf(buffer, MAXC_CHAR, "%s", colon);
					cpu->frequency = buffer;
				}
			}
			int err = fclose(fp);
			if (err != 0) perror("error closing /proc/cpuinfo");
		}
	}

	fclose(fp);
	fp = NULL;

#define delay 1

	fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	if (!fp)
		printf("error reading /proc/cpuinfo");

	while (fgets(buffer, sizeof(buffer), fp)) {
		int a = atoi(buffer);
		a /= 1000;
		cpu->temperature = a;
		fflush(stdout);
	}
	fclose(fp);
}


void
memory_data(Ram *ram) {

	struct sysinfo info;

	if (sysinfo(&info) != 0) {
		perror("sysinfo");
		return;
	}

	long total_ram = info.totalram * info.mem_unit; 

	if (sysinfo(&info) != 0) {
		perror("sysinfo");
	}

	long free_ram = info.freeram * info.mem_unit;

	ram->available	= free_ram / pow(10, 8);
	ram->total		= total_ram /  pow(10, 8);

}

void
disk_data(Disk *disk) 
{

	// TODO: return disk data

}


void
device_data(Device *device) {
	struct sysinfo info;
	if (sysinfo(&info) == -1)
		perror("sysinfo");

	device->uptime = info.uptime;
	// device->hostname = info.procs;
	device->procs = info.procs;	
	// TODO: get device model
	// TODO: get distribution and os version 
}

