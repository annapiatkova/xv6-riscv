#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"

int
mutexalloc(struct file **f)
{
  printf("creating a mutex...\n");
  if((*f = filealloc()) == 0)
    goto bad;
  if(((*f)->mutex = (struct sleeplock*)kalloc()) == 0)
    goto bad;
  initsleeplock((*f)->mutex, "mutex");
  (*f)->type = FD_MUTEX;
  (*f)->readable = 0;
  (*f)->writable = 0;
  return 0;

 bad:
  if((*f)) {
    if ((*f)->mutex) {
      kfree((char*)(*f)->mutex);
    }
    fileclose((*f));
  }
  return -1;
}

void
mutexclose(struct sleeplock *lock)
{
  printf("closing the mutex...\n");
  kfree((char*)lock);
}
