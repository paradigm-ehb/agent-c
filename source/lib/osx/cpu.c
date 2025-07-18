#ifdef __APPLE__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/sysctl.h>

float cpu_frequency(void);
char* cpu_name(void);
void cpu_temperature(void);

float cpu_frequency(void)
{
    uint64_t freq = 0;
    size_t size = sizeof(freq);

    if (sysctlbyname("hw.cpufrequency", &freq, &size, NULL, 0) < 0)
    {
        perror("sysctl");
    }
    return freq;
}

char* cpu_name(void)
{
    char* cpu_name;
    size_t size = sizeof(cpu_name);
    if (sysctlbyname("ker.hostname", &cpu_name, &size, NULL, 0) < 0)
        perror("sysctl"); 

    return cpu_name;  
}

void cpu_temperature(void)
{
    // Placeholder; macOS does not expose CPU temp directly without SMC/hacks return; 
}
#endif

