#include "kernel/types.h"
#include "user.h"
#include "kernel/riscv.h"

int main() {
  printf("\nall pages:\n");
  printpages(0, 0, 0);
  char buf1[12] = "hello world";
  printf("\nallocated a buffer (buf1) on the stack, its pages:\n");
  printpages((void*)buf1, 12, 0);
  char* buf2 = (char*)malloc(20000);
  printf("\nallocated a buffer (buf2) in the heap, its pages:\n");
  printpages((void*)buf2, 20000, 0);
  buf2[9000] = 'a';
  printf("\nchanged an element of buf2\ndirty buf2's pages:\n");
  printpages((void*)buf2, 20000, PTE_D);
  printf("\naccessed an element of buf2 without changing it (it was %d)\naccessed buf2's pages:\n", buf2[18000]);
  printpages((void*)buf2, 20000, PTE_A);
  removeflag(0, 0, PTE_D);
  printf("\nremoved the D flag from all pages\nbuf2's pages:\n");
  printpages((void*)buf2, 20000, 0);
  removeflag((void*)buf2, 20000, PTE_A);
  printf("\nremoved the A flag from buf2's pages\nbuf2's pages\n");
  printpages((void*)buf2, 20000, 0);
}
