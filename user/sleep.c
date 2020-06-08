#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("sleep: missing operand\n");
        exit();
    }

    // TODO: add check for non number input
    int sleepSec = atoi(argv[1]);

    sleep(sleepSec * 10);

    exit();
}
