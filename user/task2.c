#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int p[2];
    if (pipe(p) == 0) {
    int pid = fork();
        if (pid > 0) {
            close(p[0]);
            for (int i = 0; i < argc; ++i) {
                write(p[1], argv[i], strlen(argv[i]));
            }
            write(p[1], "\n", 1);
            close(p[1]);
            wait((int*) 0);
            exit(0);
        } else if (pid == 0) {
            close(p[1]);
            close(0);
            dup(p[0]);
            close(p[0]);
            char* argv[] = {"/wc", 0};
            exec("/wc", argv);
            exit(0);
        } else {
            write(2, "fork error\n", 11);
            exit(-1);
        }
    } else {
        write(2, "pipe error\n", 11);
        exit(-1);
    }
}
