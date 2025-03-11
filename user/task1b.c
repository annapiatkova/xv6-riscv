#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pid = fork();
    if (pid > 0) {
        if (kill(pid) == 0) {
		int status;
		pid = wait(&status);
        	printf("child %d was killed, exit status %d\n", pid, status);
        	exit(0);
        } else {
        	write(2, "kill error\n", 11);
        	exit(1);
        }
    } else if (pid == 0) {
        sleep(20);
        exit(1);
    } else {
        write(2, "fork error\n", 11);
        exit(1);
    }
}
