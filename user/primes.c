#include "kernel/types.h"
#include "user/user.h"

int startReader(int);

void writeEOF(int in)
{
    int i = 0;
    write(in, &i, sizeof(i));
}

int fork1(void)
{
    int pid;

    pid = fork();
    return pid;
}

int *getPipes()
{
    static int fdsIn[2];
    pipe(fdsIn);
    return fdsIn;
}

int startCounter()
{
    int *pipes = getPipes();
    int in = pipes[1];
    int out = pipes[0];
    int i = 0;

    // start new process
    if (fork1() == 0)
    {
        startReader(out);
        exit();
    }

    // TODO: read about constanst and type casting in C for removing this hardcode
    for (i = 2; i <= 35; i++)
    {
        write(in, &i, sizeof(i));
    }
    writeEOF(in);
    close(in);
    wait();

    return out;
}

int startReader(int in)
{
    int num;
    int i = 0;
    int devider = 0;
    int isForked = 0;
    int forkedIn = 0;
    int forkedOut = 0;

    for (;; i++)
    {
        read(in, &num, sizeof(num));
        if (num == 0) // EOF
        {
            if (isForked == 1)
            {
                writeEOF(forkedIn);
                close(forkedIn);
                wait();
                close(forkedOut);
            }
            else
            {
                close(in);
            }
            break;
        }

        if (i == 0)
        {
            printf("prime %d\n", num);
            devider = num;
            continue;
        }

        // check is number devide by devider
        if (num % devider == 0)
        {
            // skipping this values
            continue;
        }

        if (isForked == 0)
        {
            int *pipes = getPipes();
            forkedIn = pipes[1];
            forkedOut = pipes[0];

            if (fork1() == 0)
            {
                startReader(forkedOut);
                exit();
            }
            isForked = 1;
        }

        write(forkedIn, &num, sizeof(num));
    }
    return 0;
}

int main(int argc, char *argv[])
{
    startCounter();
    exit();
}