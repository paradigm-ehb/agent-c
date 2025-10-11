#ifdef __gnu_linux__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <math.h>

#define MAXC 1024
#define MAXC_CHAR 256

typedef struct {

  int frequency;
  char* name;
  int threads;

} cpu_s;

typedef struct {

  unsigned long mem_size;

} ram_s;

unsigned long get_total(void);
unsigned long get_usage(void);
long device_up_time(void);
void name();
void temperature();
void freq();
void *cpu_i(void *a);
void *ram_i(void *a);
void cpu_name(void);
void cpu_temperature(unsigned short delay);
char* cpu_frequency(void);

cpu_s _cpu;
ram_s _ram;

void *cpu_name(){
  int buffer_size = 256;
  char cpu_name[buffer_size];

  printf("Opening the CPU information files");
  FILE *fp = fopen("/proc/cpuinfo", "r");
  if (!fp)
    printf("can't open /proc/cpuinfo");

  char line[buffer_size];
  while (fgets(line, sizeof(line), fp))
  {
    if (strncmp(line, "model name", 10) == 0)
    {
      char *colon = strchr(line, ':');
      if (colon)
      {
        snprintf(cpu_name, buffer_size, "%s", colon + 2);
        cpu_name[strcspn(cpu_name, "\n")] = 0;
        int err = fclose(fp);
        if (err != 0)
          printf("error closing /proc/cpuinfo");
      }
    }
  }

  _cpu.name = cpu_name;

  return NULL;
}

char* cpu_frequency() {
  char* buffer = malloc(MAXC_CHAR);

  FILE *fp = fopen("/proc/cpuinfo", "r");
  if (!fp) {
    printf("can't open /proc/cpuinfo");
    return NULL;
  }

  while (fgets(buffer, MAXC_CHAR, fp)) {
    if (strstr(buffer, "cpu MHz") != NULL)
    {
      char *colon = strchr(buffer, ':');
      if (colon)
      {
        buffer[strcspn(buffer, "\n")] = 0;
        snprintf(buffer, MAXC_CHAR, "%s", colon);
      }
    }

    fclose(fp);
    return buffer;
  }
  return NULL;
}

void *cpu_i(void *a){
  printf("thread is working");

  return(NULL);
}

unsigned long get_total(void){

  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return 1;
  }

  long total_ram = info.totalram * info.mem_unit;
  return total_ram;

}

unsigned long get_usage(void)
{
  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return 1;
  }

  long total_ram = info.totalram * info.mem_unit;
  long free_ram = info.freeram * info.mem_unit;

  return total_ram - free_ram;
}

void *ram_i(void *a){

  printf("thread is working");
  return NULL;
}

void cpu_name(void)
{
  int buffer_size = 256;
  char cpu_name[buffer_size];

  FILE *fp = fopen("/proc/cpuinfo", "r");
  if (!fp)
    printf("can't open /proc/cpuinfo");

  char line[buffer_size];
  while (fgets(line, sizeof(line), fp))
  {
    if (strncmp(line, "model name", 10) == 0)
    {
      char *colon = strchr(line, ':');
      if (colon)
      {
        snprintf(cpu_name, buffer_size, "%s", colon + 2);
        cpu_name[strcspn(cpu_name, "\n")] = 0;
        int err = fclose(fp);
        if (err != 0)
          printf("error closing /proc/cpuinfo");

      }
    }
  }

  printf("%s", cpu_name);
  snprintf(cpu_name, buffer_size, "%s", cpu_name);
}

void cpu_temperature(unsigned short delay)
{
  while (1)
  {
    sleep(delay);
    FILE *pf = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!pf)
      printf("error reading /proc/cpuinfo");

    char buffer[MAXC];
    while (fgets(buffer, sizeof(buffer), pf))
    {
      int a = atoi(buffer);
      a /= 1000;
      printf("%dC\n", a);
      fflush(stdout);
    }
    fclose(pf);
  }
}

char* cpu_frequency(void) {
  char* buffer = malloc(MAXC_CHAR);

  FILE *fp = fopen("/proc/cpuinfo", "r");
  if (!fp) {
    printf("can't open /proc/cpuinfo");
    return NULL;
  }

  while (fgets(buffer, MAXC_CHAR, fp)) {
    if (strstr(buffer, "cpu MHz") != NULL)
    {
      char *colon = strchr(buffer, ':');
      if (colon)
      {
        buffer[strcspn(buffer, "\n")] = 0;
        snprintf(buffer, MAXC_CHAR, "%s", colon);
      }
    }

    fclose(fp);
    return buffer;
  }
  return NULL;
}

unsigned long get_total(void){

  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return 1;
  }

  long total_ram = info.totalram * info.mem_unit; 
  return total_ram;

}

unsigned long get_usage(void) 
{
  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return 1;
  }

  long total_ram = info.totalram * info.mem_unit; 
  long free_ram = info.freeram * info.mem_unit;   

  return total_ram - free_ram;
}

long device_up_time(void){
  struct sysinfo info;
  if (sysinfo(&info) == -1)
    perror("sysinfo");

  return info.uptime;
}

#endif
#ifdef __APPLE__

#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <cjson/cJSON.h>

#define CONVERT_BYTES_TO_GIGABYTES 107374182   
#define D 1073741824


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



typedef struct {

  cpu_s* cpu;
  ram_s* ram;
  disk_s* disk;
  device_s* device;

} info;

typedef struct {
  unsigned long mem_size;
} ram;

void* cpu_name();
void* cpu_thread_count();
unsigned long get_total(void);
unsigned long get_usage(void);
long device_up_time(void);
float cpu_frequency(void);
void size();
void av_size();
void* ram_i();

ram_s data;
cpu_s cpu;
device_s device_info;

void *cpu_name() {

  char *name;
  size_t len = 0;

  if (sysctlbyname("machdep.cpu.brand_string", NULL, &len, NULL, 0) < 0)
    perror("errorn in assigning the size for the cpu name variable\n");

  name = malloc(len);

  if(sysctlbyname("machdep.cpu.brand_string", name, &len, NULL, 0) < 0){
    perror("error in assigning the value to the cpu name variable\n");

    free(name);
    return NULL;
  }

  cpu.name = name;
  return NULL;
}

void *cpu_threads() {

  int count;
  size_t len = sizeof(count);
  if (sysctlbyname("machdep.cpu.thread_count", &count, &len, NULL, 0) < 0)
    perror("error in retrieving the cpu threads count\n");

  cpu.threads = count;
  return NULL;
}

void *cpu_info() {

  cpu_threads();
  cpu_name();

  printf("cpu name: %s\ncpu threads: %d\n", cpu.name, cpu.threads);
  return NULL;
}


void *device_name(){

  char *name;
  size_t size = 0;

  if (sysctlbyname("kern.hostname", NULL, &size, NULL, 0) < 0)
    perror("failed retrieving the hostname: \n"); 

  name = malloc(size);

  if(sysctlbyname("kern.hostname", name, &size, NULL, 0) < 0){
    perror("failed retrieving the hostname: \n");
    free(name);
    return NULL;
  }

  device_info.name = name;
  return NULL;
}


void *device_model(){

  char *model_name;
  size_t size = 0;

  if (sysctlbyname("hw.model", NULL, &size, NULL, 0) < 0)
    perror("failed retrieving the hostname: \n"); 

  model_name = malloc(size);

  if(sysctlbyname("hw.model", model_name, &size, NULL, 0) < 0){
    perror("failed retrieving the hostname: \n");
    free(model_name);
    return NULL;
  }

  device_info.model = model_name;
  return NULL;

}

void *device_up_time()
{
  return NULL;
}

void *device_os_version() {

  char *os_version;
  size_t size = 0;

  if (sysctlbyname("kern.ostype", NULL, &size, NULL, 0) < 0)
    perror("failed retrieving the hostname: \n"); 

  os_version = malloc(size);

  if(sysctlbyname("kern.ostype", os_version, &size, NULL, 0) < 0){
    perror("failed retrieving the os version: \n");
    free(os_version);
    return NULL;
  }

  device_info.model = os_version;
  return NULL;


}

void *get_device_info() {
  device_name();
  device_model();
  printf("device name: %s\ndevice model: %s\n", device_info.name, device_info.model);
  return NULL;
}

void size() {
  int64_t size;
  size_t len = sizeof(size);
  if (sysctlbyname("hw.memsize", &size, &len, NULL, 0) < 0)
    perror("error in retrieving the memory size");

  data.total = size / D;
  return NULL;
}

void av_size() {
  int64_t size;
  size_t len = sizeof(size);
  if (sysctlbyname("hw.memsize_usable", &size, &len, NULL, 0) < 0)
    perror("error in retrieving the available memory size");

  data.available = size / D;
  return NULL;
}

void ram_info() {
  size();
  av_size();

  printf("available ram: %LF\n", data.available);
  printf("total ram: %LF\n", data.total);

  return NULL;
}

void* ram_i(){

  printf("thread is working");
  return NULL;
}


float cpu_frequency(void){
  uint64_t freq = 0;
  size_t size = sizeof(freq);

  if (sysctlbyname("hw.cpufrequency", &freq, &size, NULL, 0) < 0)
  {
    perror("sysctl");
  }
  return freq;
}

char* cpu_name(void){

  size_t size = 0;

  if (sysctlbyname("machdep.cpu.brand_string", NULL, &size, NULL, 0) < 0)
    perror("sysctl"); 


  char *name = malloc(size);

  if(sysctlbyname("machdep.cpu.brand_string", &name, &size, NULL, 0) < 0){
    perror("sysctl");
    free(name);
    return NULL;
  }

  return name;  
}

long device_up_time(void){

  struct timeval boottime;
  size_t len = sizeof(boottime);

  if (sysctlbyname("kern.boottime", &boottime, len, NULL, 0) == -1){
    perror("sysctl error");
    return 1;
  }
}

unsigned long get_usage(void) {

  struct rusage usage;
  if(0 == getrusage(RUSAGE_SELF, &usage))
    return usage.ru_maxrss / CONVERT_BYTES_TO_GIGABYTES ; 
  else
    return 0;
}

unsigned long get_mem(void){

  int mib[2];
  size_t size;
  uint64_t ram_size;

  mib[0] = CTL_HW;
  mib[1] = HW_MEMSIZE;

  size = sizeof(ram_size);
  if (sysctl(mib, 2, &ram_size, &size, NULL, 0) == -1) {
    perror("sysctl");
  }

  return ram_size;

}

#endif
