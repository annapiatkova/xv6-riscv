#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pid = fork();
    if (pid > 0) {
        printf("parent's pid: %d\n", getpid());
        printf("child's pid: %d\n", pid);
        int status;
        pid = wait(&status);
        printf("child %d has exited with status %d\n", pid, status);
        exit(0);
    } else if (pid == 0) {
        sleep(20);
        exit(1);
    } else {
        write(2, "fork error\n", 11);
        exit(1);
    }
}
