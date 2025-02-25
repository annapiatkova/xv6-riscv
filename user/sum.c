#include "kernel/types.h"
#include "user/user.h"

int main() {
	char buf[20];
	char* i = buf;
	while(read(0, i, 1)) {
		if (*i++ == ' ') {
			break;
		}
	}
	*i = '\0';
	int a = atoi(buf);
	i = buf;
	while(read(0, i, 1)) {
		if (*i++ == '\n') {
			break;
		}
	}
	*i = '\0';
	int b = atoi(buf);
	printf("%d\n", a + b);
	return 0;
}
