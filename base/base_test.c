#include <unistd.h>
#include "base_test.h"

void
write_int(i32 num)
{
    if (num < 0)
    {
        write(STDERR_FILENO, "-", 1);
        num = -num;
    }
    if (num >= 10)
        write_int(num / 10);
    char digit = '0' + (num % 10);

    write(STDERR_FILENO, &digit, 1);
}
