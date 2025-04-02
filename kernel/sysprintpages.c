#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"

char index_hex[4];
char pa_hex[17];

char* to_hex_3(int i) {
  for (int j = 2; j >= 0; --j) {
    if (i % 16 < 10) {
      index_hex[j] = '0' + (i % 16);
    } else {
      index_hex[j] = 'A' + (i % 16 - 10);
    }
    i /= 16;
  }
  return index_hex;
}

char* to_hex_16(pte_t i) {
  for (int j = 15; j >= 0; --j) {
    if (i % 16 < 10) {
      pa_hex[j] = '0' + (i % 16);
    } else {
      pa_hex[j] = 'A' + (i % 16 - 10);
    }
    i /= 16;
  }
  return pa_hex;
}

int need_to_print(int flag, pte_t pte) {
  if (flag == 0) {
    return 1;
  }
  return pte & flag;
}

uint64 sys_printpages(void) {
  index_hex[3] = '\0';
  pa_hex[16] = '\0';
  uint64 buf;
  argaddr(0, &buf);
  int len;
  argint(1, &len);
  int flag;
  argint(2, &flag);
  if (flag != 0 && flag != PTE_A && flag != PTE_D) {
    return 1;
  }
  struct proc *p = myproc();
  pagetable_t pt = p->pagetable;
  printf("PAGETABLE 0x%s\n", to_hex_16((pte_t)pt));

  char flags[8];
  flags[7] = '\0';
  if (buf == 0 || len == 0) {
    for (int i = 0; i < 512; i++) {
      pte_t pte = pt[i];
      if (pte & PTE_V) {
        flags[0] = (pte & PTE_R) ? 'R' : '_';
        flags[1] = (pte & PTE_W) ? 'W' : '_';
        flags[2] = (pte & PTE_X) ? 'X' : '_';
        flags[3] = (pte & PTE_U) ? 'U' : '_';
        flags[4] = (pte & PTE_G) ? 'G' : '_';
        flags[5] = (pte & PTE_A) ? 'A' : '_';
        flags[6] = (pte & PTE_D) ? 'D' : '_';
        printf("0x%s -> 0x%s %s\n", to_hex_3(i), to_hex_16(pte), flags);
        pagetable_t pt2 = (pagetable_t)PTE2PA(pte);
        for (int j = 0; j < 512; j++) {
          pte_t pte2 = pt2[j];
          if (pte2 & PTE_V) {
            flags[0] = (pte2 & PTE_R) ? 'R' : '_';
            flags[1] = (pte2 & PTE_W) ? 'W' : '_';
            flags[2] = (pte2 & PTE_X) ? 'X' : '_';
            flags[3] = (pte2 & PTE_U) ? 'U' : '_';
            flags[4] = (pte2 & PTE_G) ? 'G' : '_';
            flags[5] = (pte2 & PTE_A) ? 'A' : '_';
            flags[6] = (pte2 & PTE_D) ? 'D' : '_';
            printf("......... 0x%s -> 0x%s %s\n", to_hex_3(j), to_hex_16(pte2), flags);
            pagetable_t pt3 = (pagetable_t)PTE2PA(pte2);
            for (int h = 0; h < 512; h++) {
              pte_t pte3 = pt3[h];
              if (pte3 & PTE_V) {
                flags[0] = (pte3 & PTE_R) ? 'R' : '_';
                flags[1] = (pte3 & PTE_W) ? 'W' : '_';
                flags[2] = (pte3 & PTE_X) ? 'X' : '_';
                flags[3] = (pte3 & PTE_U) ? 'U' : '_';
                flags[4] = (pte3 & PTE_G) ? 'G' : '_';
                flags[5] = (pte3 & PTE_A) ? 'A' : '_';
                flags[6] = (pte3 & PTE_D) ? 'D' : '_';
                if (need_to_print(flag, pte3)) {
                  printf("...................0x%s -> 0x%s %s\n", to_hex_3(h), to_hex_16(pte3), flags);
                }
              }
            }
          }
        }
      }
    }
  } else {
    for (int i = PX(2, buf); i <= PX(2, buf + len - 1); i++) {
      pte_t pte = pt[i];
      if (pte & PTE_V) {
        flags[0] = (pte & PTE_R) ? 'R' : '_';
        flags[1] = (pte & PTE_W) ? 'W' : '_';
        flags[2] = (pte & PTE_X) ? 'X' : '_';
        flags[3] = (pte & PTE_U) ? 'U' : '_';
        flags[4] = (pte & PTE_G) ? 'G' : '_';
        flags[5] = (pte & PTE_A) ? 'A' : '_';
        flags[6] = (pte & PTE_D) ? 'D' : '_';
        printf("0x%s -> 0x%s %s\n", to_hex_3(i), to_hex_16(pte), flags);
        pagetable_t pt2 = (pagetable_t)PTE2PA(pte);
        for (int j = PX(1, buf); j <= PX(1, buf + len - 1); j++) {
          pte_t pte2 = pt2[j];
          if (pte2 & PTE_V) {
            flags[0] = (pte2 & PTE_R) ? 'R' : '_';
            flags[1] = (pte2 & PTE_W) ? 'W' : '_';
            flags[2] = (pte2 & PTE_X) ? 'X' : '_';
            flags[3] = (pte2 & PTE_U) ? 'U' : '_';
            flags[4] = (pte2 & PTE_G) ? 'G' : '_';
            flags[5] = (pte2 & PTE_A) ? 'A' : '_';
            flags[6] = (pte2 & PTE_D) ? 'D' : '_';
            printf("......... 0x%s -> 0x%s %s\n", to_hex_3(j), to_hex_16(pte2), flags);
            pagetable_t pt3 = (pagetable_t)PTE2PA(pte2);
            for (int h = PX(0, buf); h <= PX(0, buf + len - 1); h++) {
              pte_t pte3 = pt3[h];
              if (pte3 & PTE_V) {
                flags[0] = (pte3 & PTE_R) ? 'R' : '_';
                flags[1] = (pte3 & PTE_W) ? 'W' : '_';
                flags[2] = (pte3 & PTE_X) ? 'X' : '_';
                flags[3] = (pte3 & PTE_U) ? 'U' : '_';
                flags[4] = (pte3 & PTE_G) ? 'G' : '_';
                flags[5] = (pte3 & PTE_A) ? 'A' : '_';
                flags[6] = (pte3 & PTE_D) ? 'D' : '_';
                if (need_to_print(flag, pte3)) {
                  printf("...................0x%s -> 0x%s %s\n", to_hex_3(h), to_hex_16(pte3), flags);
                }
              } else {
                return 1;
              }
            }
          } else {
            return 1;
          }
        }
      } else {
        return 1;
      }
    }
  }
  return 0;
}

uint64 sys_removeflag(void) {
  index_hex[3] = '\0';
  pa_hex[16] = '\0';
  uint64 buf;
  argaddr(0, &buf);
  int len;
  argint(1, &len);
  int flag;
  argint(2, &flag);
  if (flag != PTE_A && flag != PTE_D) {
    return 1;
  }
  struct proc *p = myproc();
  pagetable_t pt = p->pagetable;
  
  if (buf == 0 || len == 0) {
    for (int i = 0; i < 512; i++) {
      pte_t pte = pt[i];
      if (pte & PTE_V) {
        pagetable_t pt2 = (pagetable_t)PTE2PA(pte);
        for (int j = 0; j < 512; j++) {
          pte_t pte2 = pt2[j];
          if (pte2 & PTE_V) {
            pagetable_t pt3 = (pagetable_t)PTE2PA(pte2);
            for (int h = 0; h < 512; h++) {
              pte_t pte3 = pt3[h];
              if (pte3 & PTE_V) {
                if (pte3 & flag) {
                  pt3[h] &= ~flag;
                }
              }
            }
          }
        }
      }
    }
  } else {  
    for (int i = PX(2, buf); i <= PX(2, buf + len - 1); i++) {
      pte_t pte = pt[i];
      if (pte & PTE_V) {
        pagetable_t pt2 = (pagetable_t)PTE2PA(pte);
        for (int j = PX(1, buf); j <= PX(1, buf + len - 1); j++) {
          pte_t pte2 = pt2[j];
          if (pte2 & PTE_V) {
            pagetable_t pt3 = (pagetable_t)PTE2PA(pte2);
            for (int h = PX(0, buf); h <= PX(0, buf + len - 1); h++) {
              pte_t pte3 = pt3[h];
              if (pte3 & PTE_V) {
                if (pte3 & flag) {
                  pt3[h] &= ~flag;
                }
              } else {
                return 1;
              }
            }
          } else {
            return 1;
          }
        }
      } else {
        return 1;
      }
    }
  }
  return 0;
}  
