#include "kernel/types.h"
#include "user.h"

int main() {
	int mutex_id = mutex();
  if (mutex_id < 0)
    write(2, "mutex error", 11);
	printf("created a mutex, fd = %d\n", mutex_id);
	char buf[10];
	printf("read returned %d\n", read(mutex_id, buf, 1));
	buf[0] = 'a';
	printf("write returned %d\n", read(mutex_id, buf, 1));
	if (mutex_lock(mutex_id) < 0){
  	write(2, "mutex error", 11);
    exit(1);
  }
	printf("acquired the lock\n");
	printf("process %d closed the mutex held by process %d, close returned %d\n", getpid(), getpid(), close(mutex_id));
	mutex_id = mutex();
  if (mutex_id < 0)
    write(2, "mutex error", 11);
	printf("created a mutex, fd = %d\n", mutex_id);
	if (mutex_lock(mutex_id) < 0){
  	write(2, "mutex error", 11);
    exit(1);
  }
	printf("acquired the lock\n");
	int parent_pid = getpid();
	int pid = fork();
  if (pid == 0) { // child
		printf("process %d closed the mutex held by process %d, close returned %d\n", getpid(), parent_pid, close(mutex_id));
		mutex_id = mutex();
		if (mutex_id < 0)
		  write(2, "mutex error", 11);
		printf("process %d created a mutex, fd = %d\n", getpid(), mutex_id);
		printf("process %d is exiting without closing its mutex\n", getpid());
		exit(0);
	} else if (pid > 0) { // parent
		wait(0);
		printf("pid %d has exited\n", pid);
		mutex_id = mutex();		
		if (mutex_id < 0)
		  write(2, "mutex error", 11);
		printf("created a mutex, fd = %d\n", mutex_id);
		pid = fork();
  	if (pid == 0) { // child
			printf("process %d is trying to acquire the lock\n", getpid());
			if (mutex_lock(mutex_id) < 0){
				write(2, "mutex error", 11);
				exit(1);
			}
			printf("process %d acquired the lock\n", getpid());
			sleep(10);
			printf("process %d is releasing the lock\n", getpid());
			mutex_unlock(mutex_id);
			exit(0);
		} else if (pid > 0) { // parent
			printf("process %d is trying to acquire the lock\n", getpid());
			if (mutex_lock(mutex_id) < 0){
				write(2, "mutex error", 11);
				exit(1);
			}
			printf("process %d acquired the lock\n", getpid());
			sleep(10);
			printf("process %d is releasing the lock\n", getpid());
			if (mutex_unlock(mutex_id) < 0){
				write(2, "mutex error", 11);
				exit(1);
			}
			sleep(11);
			mutex_id = mutex();		
			if (mutex_id < 0)
				write(2, "mutex error", 11);
			printf("process %d created a mutex, fd = %d\n", getpid(), mutex_id);
			if (mutex_lock(mutex_id) < 0){
				write(2, "mutex error", 11);
				exit(1);
			}
			printf("process %d acquired the lock of mutex %d\n", getpid(), mutex_id);
			printf("process %d is exiting without closing its mutex or releasing the lock\n", getpid());
			exit(0);
		} else {
			write(2, "fork error", 10);
			exit(1);
		}
		exit(0);
	} else {
    write(2, "fork error", 10);
		exit(1);
  }
}
