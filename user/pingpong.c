#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int fdsIn[2];
    char bufIn[100];
    char *pingText = "ping";
    char *pongText = "pong";

    int fdsOut[2];
    char bufOut[100];

    pipe(fdsIn);
    pipe(fdsOut);

    int pid = fork();
    if (pid > 0) {
        write(fdsIn[1], pingText, strlen(pingText));
        wait();

        read(fdsOut[0], bufOut, sizeof(bufOut));
        int ppid = getpid();
        printf("%d: received %s\n", ppid, bufOut);
    } else if (pid == 0) {
        read(fdsIn[0], bufIn, sizeof(bufIn));

        int ppid = getpid();
        printf("%d: received %s\n", ppid, bufIn);

        write(fdsOut[1], pongText, strlen(pongText));
        exit();
    } else {
    }
    exit();
}