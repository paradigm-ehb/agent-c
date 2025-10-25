#ifndef TYPES_H
#define TYPES_H

typedef struct {

  char* name;
  char* frequency;
  int temperature;
  int threads;

} cpu_s;

typedef struct {
  unsigned long total;
  unsigned long available;
} ram_s;

typedef struct {
  long long size;
  short name;
} disk_s;

typedef struct { 

  char *name;
  char *hostname;
  char *os_version;
  int uptime;
  cpu_s cpu;
  ram_s ram;
  disk_s disk;


} device_s;


extern cpu_s *u_cpu ;
extern ram_s *u_ram ;
extern disk_s *u_disk ;
extern device_s *u_device ;



#endif
