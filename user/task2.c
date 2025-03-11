#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int p[2];
    if (pipe(p) == 0) {
    int pid = fork();
        if (pid > 0) {
            if (close(p[0])) {
                write(2, "close error\n", 12);
                exit(1);
            }
            for (int i = 0; i < argc; ++i) {
                int r = write(p[1], argv[i], strlen(argv[i]));
                if (r < strlen(argv[i])) {
                    int n = r;
                    while (n < strlen(argv[i])) {
                        if (r < 0) {
                            write(2, "write error\n", 12);
                            exit(1);
                        }
                        r = write(p[1], argv[i] + n, strlen(argv[i]) - n);
                        n += r;
                    }
                }
            }
            int r = write(p[1], "\n", 1);
            while (r < 1) {
                if (r < 0) {
                    write(2, "write error\n", 12);
                    exit(1);
                }
                r = write(p[1], "\n", 1);
            }
            if (close(p[1])) {
                write(2, "close error\n", 12);
                exit(1);
            }
            wait((int*) 0);
            exit(0);
        } else if (pid == 0) {
            if (close(p[1])) {
                write(2, "close error\n", 12);
                exit(1);
            }
            if (close(0)) {
                write(2, "close error\n", 12);
                exit(1);
            }
            if (dup(p[0])) {
                write(2, "dup error\n", 10);
                exit(1);
            }
            if (close(p[0])) {
                write(2, "close error\n", 12);
                exit(1);
            }
            char* argv[] = {"/wc", 0};
            exec("/wc", argv);
            exit(0);
        } else {
            write(2, "fork error\n", 11);
            exit(1);
        }
    } else {
        write(2, "pipe error\n", 11);
        exit(1);
    }
}
