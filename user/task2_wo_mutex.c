#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]) {
  int pid = fork();
  if (pid == 0) { // child
    // printing chars as strings because printf in xv6 doesn't support %c
    char ch[2];
    ch[1] = '\0';
    for (int i = 0; i < argc; ++i){
      for (int j = 0; j < strlen(argv[i]); ++j){
        ch[0] = argv[i][j];
        printf("%d: arg %d, char '%s'\n", getpid(), i, ch);
      }
    }
    exit(0);
  } else if (pid > 0) { // parent
    char ch[2];
    ch[1] = '\0';
    for (int i = 0; i < argc; ++i){
      for (int j = 0; j < strlen(argv[i]); ++j){
        ch[0] = argv[i][j];
        printf("%d: arg %d, char '%s'\n", getpid(), i, ch);
      }
    }
    wait(0);
    exit(0);
  } else {
    write(2, "fork error", 10);
    exit(1);
  }
}
