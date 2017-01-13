#include <stdio.h>
#include <stdlib.h>

#ifndef WIN32
	#include <unistd.h>
#else
#include <windows.h>
#define sleep(t) Sleep((t)*1000)
#endif

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
