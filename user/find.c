#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *getname(char *path)
{
    static char buf[512];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;

    memset(buf, 0, sizeof(buf)); // force clear buffer
    memmove(buf, p, strlen(p));
    return buf;
}

void find(char *path, char *matchName)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_FILE)
    {
        printf("%s\n", buf);
        close(fd);
        return;
    }
    if (st.type == T_DEVICE)
    {
        printf("not processing device files\n");
        close(fd);
        return;
    }

    // processing directories
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
        close(fd);
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue; // skip files with 0 links

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            continue;
        }

        char *objectName = getname(buf);
        if (strcmp(objectName, ".") == 0 || strcmp(objectName, "..") == 0)
        {
            continue;
        }

        if (strcmp(objectName, matchName) == 0)
        {
            printf("%s\n", buf);
        }

        if (st.type == T_DIR)
        {
            find(buf, matchName);
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    find(argv[1], argv[2]);
    exit();
}
