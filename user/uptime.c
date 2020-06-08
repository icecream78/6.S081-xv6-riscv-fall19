#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int time = uptime();
    // this crutch made for correct translating time between cycles in seconds
    // in file kernel/start.c interval described as 1000000 as mean as near 1/10th second in qemu.
    int seconds = time / 10;

    printf("Time ellapsed from system startup: %d s\n", seconds);
    exit();
}
