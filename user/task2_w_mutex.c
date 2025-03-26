#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]) {
  int mutex_id = mutex();
  if (mutex_id < 0) {
    write(2, "mutex error", 11);
  }
  int pid = fork();
  if (pid == 0) { // child
    // printing chars as strings because printf in xv6 doesn't support %c
    char ch[2];
    ch[1] = '\0';
    for (int i = 0; i < argc; ++i){
      for (int j = 0; j < strlen(argv[i]); ++j){
        ch[0] = argv[i][j];
        if (mutex_lock(mutex_id) < 0){
          write(2, "mutex error", 11);
          exit(1);
        }
        printf("%d: arg %d, char '%s'\n", getpid(), i, ch);
        if (mutex_unlock(mutex_id) < 0){
          write(2, "mutex error", 11);
          exit(1);
        }
      }
    }
    exit(0);
  } else if (pid > 0) { // parent
    char ch[2];
    ch[1] = '\0';
    for (int i = 0; i < argc; ++i){
      for (int j = 0; j < strlen(argv[i]); ++j){
        ch[0] = argv[i][j];
        if (mutex_lock(mutex_id) < 0){
          write(2, "mutex error", 11);
          exit(1);
        }
        printf("%d: arg %d, char '%s'\n", getpid(), i, ch);
        if (mutex_unlock(mutex_id) < 0){
          write(2, "mutex error", 11);
          exit(1);
        }
      }
    }
    wait(0);
    close(mutex_id);
    exit(0);
  } else {
    write(2, "fork error", 10);
    exit(1);
  }
}
