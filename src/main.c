#include <bits/time.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define error(ret, s, ...)                   \
	do {                                     \
		fprintf(stderr, (s), ##__VA_ARGS__); \
		exit(ret);                           \
	} while (0)

typedef unsigned long long ull;

/* acquires the random data */
long rdat(void) {
	return random();
}

int main(int argc, char** argv) {
	// get the current time to set as the seed
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	srandom(ts.tv_nsec);

	// return the result if no input
	if (argc <= 1) return printf("%s\n", (random() & 1) ? "heads" : "tails");


	// loop through arguments
	for (unsigned i = 1; i < (unsigned)argc; ++i) {
		// parse the argument
		errno = 0;
		ull c = strtoull(argv[i], NULL, 10);
		if (errno != 0) error(errno, "parse error for string: '%s'\n", argv[i]);
		if (c < 1 || c > (ULLONG_MAX - 1)) error(1, "count may only be in between 1 and %llu. Received %llu\n", ULLONG_MAX - 1, c);

		// perform for the input count
		ull headsc = 0; // amount of heads
		ull tailsc = 0; // amount of tails


		for (ull j = 0; j < c; ++j) {
			ull n = rdat();

#if defined(__GNUC__) && __GNUC__ >= 8
# pragma GCC unroll 31
#elif defined(__clang__)
# pragma unroll 31
#endif
			for (ull b = 1; b <= INT32_MAX; b <<= 1)
				(void)((n & b) ? headsc++ : tailsc++); // increment the correct counter
		}

		// print the results of this cycle
		printf("results:\n heads: %llu\n tails: %llu\n", headsc, tailsc);
	}
	return 0;
}
