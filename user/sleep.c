#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("sleep: missing operand\n");
        exit();
    }

    if (strisint(argv[1]) != 1) {
        printf("non numberic value provided\n");
        exit();
    }

    int sleepSec = atoi(argv[1]); // parse string to int value

    sleep(sleepSec * 10); // make syscall with seconds count

    exit();
}
