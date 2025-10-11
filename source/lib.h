#ifndef LIB_H
#define LIB_H

#ifdef __APPLE__

typedef struct{
    char* name;
} device_s;

typedef struct {

  int frequency;
  char* name;
  int threads;

} cpu_s;

typedef struct {
    long double total;
    long double available;
} ram_s;

typedef struct {
    long long size;
    short name;
} disk_s;

extern device_s device_info;

void* get_device_info();
void* disk_size();
void* size();
void* av_size();
void* ram_info();


float cpu_frequency(void);
char* cpu_name(void);
void *cpu_name();
void *cpu_threads();
void *cpu_info();

#endif

#ifdef __gnu_linux__

void cpu_name();
void cpu_freq();
void cpu_temperature();
void *cpu_i(void *a);
void size();
void av_size();
void *ram_i(void *a);
float cpu_freq(void);
char* cpu_name(void);
void cpu_temperature(void);

#endif

