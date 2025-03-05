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
            //read(0, 
            char buf[1024];
            int n;
            while (n = read(0, &buf, 1024)) {
                if (n < 0) {
                    write(2, "read error\n", 11);
                    exit(-1);
                }
                write(1, &buf, n);
            }
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
