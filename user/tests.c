#include "kernel/types.h"
#include "user/user.h"

int buf[8] = {};

void print_n_bytes(int n) {
  if (n == 0) {
    printf("{}\n");
    return;
  }
  printf("{");
  for (int i = 0; i < n - 1; ++i)
    printf("%d, ", buf[i]);
  printf("%d}\n", buf[n - 1]);
}

int main() {
  int retval;
  retval = write(3, &buf, 1); // null
  printf("wrote 1 byte to null: retval = %d\n", retval);
  retval = write(4, &buf, 1); // zero
  printf("wrote 1 byte to zero: retval = %d\n", retval);
  retval = write(5, &buf, 1); // urandom
  printf("wrote 1 byte to urandom: retval = %d\n", retval);
  retval = write(6, &buf, 1); // nullstat
  printf("wrote 1 byte to nullstat: retval = %d\n", retval);
  
  retval = read(3, &buf, 1); // null
  printf("read 1 byte from null: retval = %d, value read = %d\n", retval, buf[0]);
  retval = read(4, &buf, 1); // zero
  printf("read 1 byte from zero: retval = %d, value read = %d\n", retval, buf[0]);
  retval = read(5, &buf, 1); // urandom
  printf("read 1 byte from urandom: retval = %d, value read = %d\n", retval, buf[0]);
  retval = read(6, &buf, 1); // nullstat
  printf("read 1 byte from nullstat: retval = %d, value read = %d\n", retval, buf[0]);
  
  retval = write(3, &buf, 8); // null
  printf("wrote 8 bytes to null: retval = %d\n", retval);
  retval = write(4, &buf, 8); // zero
  printf("wrote 8 bytes to zero: retval = %d\n", retval);
  retval = write(5, &buf, 8); // urandom
  printf("wrote 8 bytes to urandom: retval = %d\n", retval);
  retval = write(6, &buf, 8); // nullstat
  printf("wrote 8 bytes to nullstat: retval = %d\n", retval);
  
  retval = read(3, &buf, 8); // null
  printf("read 8 byte from null: retval = %d, values read = ", retval);
  print_n_bytes(retval);
  retval = read(4, &buf, 8); // zero
  printf("read 8 bytes from zero: retval = %d, value read = ", retval);
  print_n_bytes(retval);
  retval = read(5, &buf, 8); // urandom
  printf("read 8 bytes from urandom: retval = %d, values read = ", retval);
  print_n_bytes(retval);
  retval = read(6, &buf, 8); // nullstat
  printf("read 8 bytes from nullstat: retval = %d, values read = ", retval);
  print_n_bytes(retval);
  return 0;
}
