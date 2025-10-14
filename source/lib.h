#ifndef LIB_H
#define LIB_H
#include "types.h" 

#ifdef __APPLE__

void cpu_name(cpu_s cpu);
void cpu_threads();
void cpu_temperature();
void cpu_frequency();

void disk_size();
void size();

void av_size();
void ram_info();

void mem_size();
void mem_av_size();

void get_device_info();
void get_os_version();

#endif
#endif

