#include <errno.h>
#include <linux/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
	// set the seed to the number of seconds xor with nanoseconds
	{
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		srand((uint32_t)(ts.tv_nsec ^ ts.tv_sec)); // combine seconds and nanoseconds
	}

	// just print whether it's heads or tails
	if (argc == 1) {
		printf("%s\n", (rand() & 1) ? "heads" : "tails");
		return 0;
	}

	// if we have input; perform coin flips for those times
	for (uint32_t i = 1; i < argc; ++i) {
		int n = 0;           // contains the random data
		uint32_t headsc = 0; // amount of heads
		uint32_t tailsc = 0; // amount of tails

		// get the integer from the string, return 1 if an error occurred
		errno = 0;
		long c = strtol(argv[i], NULL, 10);
		if (errno == EINVAL) {
			fprintf(stderr, "syntax error for string: %s\n", argv[i]);
			return 1;
		}

		// perform for the input count
		for (long j = 0; j < c; ++j) {
			if (n == 0) n = rand();                // get a new random number if the random data ran out
			(void)((n & 1) ? headsc++ : tailsc++); // increment the correct counter
			n >>= 1;                               // shift the random integer right by 1
		}

		// print the results of this cycle
		printf("results:\n heads: %u\n tails: %u\n", headsc, tailsc);
	}
}
