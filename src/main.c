#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((uint32_t)(ts.tv_nsec ^ ts.tv_sec));  // combine seconds and nanoseconds
    printf("%s\n", (rand() & 1) ? "heads" : "tails");
    return 0;
}
