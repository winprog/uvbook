#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32

#else
	#include <unistd.h>
#endif

#if 0
int main() {
    int i;
    for (i = 0; i < 10; i++) {
        printf("tick\n");
        fflush(stdout);
        sleep(1);
    }
    printf("BOOM!\n");
    return 0;
}
#endif