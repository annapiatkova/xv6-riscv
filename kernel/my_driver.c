#include "types.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "riscv.h"
#include "defs.h"

struct {
  struct spinlock lock;
  
  uint seed;
  uint a;
  uint c;
  uint m;
} urandom;

struct {
  struct spinlock lock;
  
  uint64 count;
} nullstat;

int
nullread(int user_dst, uint64 dst, int n)
{
  return 0;
}

int
nullwrite(int user_src, uint64 src, int n)
{
  return n;
}

int
zeroread(int user_dst, uint64 dst, int n)
{
  int target = n;
  int buf[1] = {0};
  while(n > 0){
    if(either_copyout(user_dst, dst, &buf, 1) == -1)
      break;

    dst++;
    --n;
  }
  return target - n;
}

int
zerowrite(int user_src, uint64 src, int n)
{
  return -1;
}

int
urandomread(int user_dst, uint64 dst, int n)
{
  int target = n;
  int buf[1];
  acquire(&urandom.lock);
  while(n > 0){
    buf[0] = (urandom.a * urandom.seed + urandom.c) % urandom.m;
    
    if(either_copyout(user_dst, dst, &buf, 1) == -1)
      break;

    urandom.seed = buf[0];
    dst++;
    --n;
  }
  release(&urandom.lock);
  return target - n;
}

int
urandomwrite(int user_src, uint64 src, int n)
{
  if (n != 1)
    return -1;
  uint c[1];
  if(either_copyin(&c, user_src, src, 1) == -1)
    return 0;
  acquire(&urandom.lock);
  urandom.seed = c[0];
  release(&urandom.lock);
  return 1;
}

int
nullstatread(int user_dst, uint64 dst, int n)
{
  if (n != sizeof(uint64))
    return -1;
  acquire(&nullstat.lock);
  uint64 buf[1] = { nullstat.count };
  release(&nullstat.lock);
  if(either_copyout(user_dst, dst, &buf, 1) == -1)
    return 0;
  return 1;
}

int
nullstatwrite(int user_src, uint64 src, int n)
{
  acquire(&nullstat.lock);
  nullstat.count += n;
  release(&nullstat.lock);
  return n;
}

#define NULL_DEVICE     0
#define ZERO_DEVICE     1
#define URANDOM_DEVICE  2
#define NULLSTAT_DEVICE 3

void
mydriverinit(void)
{
  initlock(&urandom.lock, "urandom");
  initlock(&nullstat.lock, "nullstat");
  
  urandom.seed = 0;
  urandom.a = 48271;
  urandom.c = 0;
  urandom.m = 2147483647;
  
  nullstat.count = 0;

  uartinit();

  // connect read and write system calls
  // to consoleread and consolewrite.
  devsw[MYDRIVER][NULL_DEVICE].read      = nullread;
  devsw[MYDRIVER][NULL_DEVICE].write     = nullwrite;
  devsw[MYDRIVER][ZERO_DEVICE].read      = zeroread;
  devsw[MYDRIVER][ZERO_DEVICE].write     = zerowrite;
  devsw[MYDRIVER][URANDOM_DEVICE].read   = urandomread;
  devsw[MYDRIVER][URANDOM_DEVICE].write  = urandomwrite;
  devsw[MYDRIVER][NULLSTAT_DEVICE].read  = nullstatread;
  devsw[MYDRIVER][NULLSTAT_DEVICE].write = nullstatwrite;
}
