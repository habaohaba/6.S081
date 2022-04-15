#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    //child to parent
    int fds_1[2];
    //parent to child
    int fds_2[2];
    pipe(fds_1);
    pipe(fds_2);
    int p = fork();
    if (p == 0) {
        //read byte from parent 
        close(fds_2[1]);
        char text[1] = "0";
        read(fds_2[0], text, 1);
        if (text[0] != 'p') {
            printf("error pingpong.\n");
            exit(1);
        }
        int pid = getpid();
        printf("%d: received ping\n", pid);
        close(fds_2[0]);
        //write byte to parent 
        close(fds_1[0]);
        write(fds_1[1], "c", 1);
        close(fds_1[1]);
        exit(0);
    } else {
        //write byte to child
        close(fds_2[0]);
        write(fds_2[1], "p", 1);
        close(fds_2[1]);
        //read byte from child
        close(fds_1[1]);
        char text[1] = "0";
        read(fds_1[0], text, 1);
        if (text[0] != 'c') {
            printf("error pingpong.\n");
            exit(1);
        }
        int pid = getpid();
        printf("%d: received pong\n", pid);
        close(fds_1[0]);
        exit(0);
    }
}

