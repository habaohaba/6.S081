#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        char forget[] = "You forget to pass an argument.\n";
        write(1, forget, strlen(forget));
        exit(1);
    } else {
        int time = atoi(argv[1]);
        sleep(time);
        exit(0);
    }
}
