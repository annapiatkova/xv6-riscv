#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int p[2];
    if (pipe(p) == 0) {
    int pid = fork();
        if (pid > 0) {
            close(p[0]);
            for (int i = 0; i < argc; ++i) {
                int r = write(p[1], argv[i], strlen(argv[i]));
                if (r < strlen(argv[i])) {
                    int n = r;
                    while (n < strlen(argv[i])) {
                        if (r < 0) {
                            perror("write error");
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
                    perror("write error");
                    exit(1);
                }
                r = write(p[1], "\n", 1);
            }
            if (close(p[1])) {
                perror("close error");
                exit(1);
            }
            wait((int*) 0);
            exit(0);
        } else if (pid == 0) {
            if (close(p[1])) {
                perror("close error");
                exit(1);
            }
            char buf[1024];
            int m;
            while (m = read(p[0], &buf, 1024)) {
                if (m < 0) {
                    perror("read error");
                    exit(1);
                }
                if (m > 0) {
                    int r = write(1, &buf, m);
                    if (r < m) {
                        int n = r;
                        while (n < m) {
                            if (r < 0) {
                                perror("write error");
                                exit(1);
                            }
                            r = write(1, &buf + n, m - n);
                            n += r;
                        }
                    }
                }
            }
            exit(0);
        } else {
            perror("fork error");
            exit(1);
        }
    } else {
        perror("pipe error");
        exit(1);
    }
}
