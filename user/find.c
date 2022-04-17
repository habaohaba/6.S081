#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "cannot open %s.\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "cannot stat %s.\n", path);
        exit(1);
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        if (de.inum == 0) {
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (stat(buf, &st) < 0) {
            fprintf(2, "cannot stat %s.\n", buf);
            exit(1);
        } 

        switch (st.type) {
            case T_FILE:
                if (strcmp(de.name, target) == 0) {
                    printf("%s\n", buf);
                }
                break;
            case T_DIR:
                if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                    find(buf, target);
                }
                break;
        }   
    }
    close(fd);
    return;
}

int main(int argc, char *argv[]) {
    //check argument numbers
    if (argc != 3) {
        fprintf(2, "wrong number of arguments.\n");
        exit(1);
    }
    char *path = argv[1];
    char *target = argv[2];
    find(path, target);
    exit(0);
}

