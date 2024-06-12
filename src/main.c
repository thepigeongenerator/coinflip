#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void) {
    srand(time(NULL)); // set the random seed to the system time
    char* result = NULL;

    if (rand() & (~1) != 0) {
        result = "heads";
    }
    else {
        result = "tails";
    }

    printf("%s\n", result);
    return 0;
}
