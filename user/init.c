// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *argv[] = { "sh", 0 };

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr
  
  if(open("null_device", O_RDWR) < 0){
    mknod("null_device", MYDRIVER, NULL_DEVICE);
    open("null_device", O_RDWR);
  }
  
  if(open("zero_device", O_RDWR) < 0){
    mknod("zero_device", MYDRIVER, ZERO_DEVICE);
    open("zero_device", O_RDWR);
  }
  
  if(open("urandom_device", O_RDWR) < 0){
    mknod("urandom_device", MYDRIVER, URANDOM_DEVICE);
    open("urandom_device", O_RDWR);
  }
  
  if(open("nullstat_device", O_RDWR) < 0){
    mknod("nullstat_device", MYDRIVER, NULLSTAT_DEVICE);
    open("nullstat_device", O_RDWR);
  }

  for(;;){
    printf("init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf("init: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      exec("sh", argv);
      printf("init: exec sh failed\n");
      exit(1);
    }

    for(;;){
      // this call to wait() returns if the shell exits,
      // or if a parentless process exits.
      wpid = wait((int *) 0);
      if(wpid == pid){
        // the shell exited; restart it.
        break;
      } else if(wpid < 0){
        printf("init: wait returned an error\n");
        exit(1);
      } else {
        // it was a parentless process; do nothing.
      }
    }
  }
}
