#include "kernel/types.h"
#include "user/user.h"

int main() {
	char buf[25];
	char* zero_str = "0";
	char* i = buf;
	while(read(0, i, 1)) {
		if (*i++ == '\n') {
			break;
		}
	}
	*--i = '\0';
	i = strchr(buf, ' ');
	if (i == 0) {
		printf("error: no space\n");
		return -1;
	}
	*i = '\0';
	int a = atoi(buf);
	if (a == 0 && strcmp(buf, zero_str) != 0) {
		printf("error: not an integer\n");
		return -1;
	}
	int b = atoi(++i);
	if (b == 0 && strcmp(i, zero_str) != 0) {
		printf("error: not an integer\n");
		return -1;
	}
	printf("%d\n", add(a, b));
	return 0;
}
