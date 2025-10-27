#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "lib.h"

cpu_s *u_cpu;
ram_s *u_ram;
disk_s *u_disk;
device_s *u_device;

void init() {

  cpu_name();
  cpu_frequency();
  cpu_threads();
  cpu_temperature();
  mem_size();
  mem_av_size();
  get_total();
  get_usage();
  device_os_version();
  device_model();
  device_up_time();
  device_model();

  u_cpu = malloc(sizeof(cpu_s));
  u_ram = malloc(sizeof(disk_s));
  u_disk = malloc(sizeof(disk_s));
  u_cpu = malloc(sizeof(device_s));

}

void end() {

  free(u_cpu);
  free(u_ram);
  free(u_disk);
  free(u_device);

}

int main() {

  init();

  printf("%d\n%s\n%s\n%d\n", u_cpu->temperature, u_cpu->frequency, u_cpu->name, u_cpu->threads);
  printf("%s\n%s\n%s\n%d", u_device->name, u_device->hostname, u_device->os_version, u_device->uptime);
  end();
  return 0;
}


