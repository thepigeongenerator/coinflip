#include <bits/time.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define error(ret, s, ...)                             \
	do {                                               \
		fprintf(stderr, (s)__VA_OPT__(, __VA_ARGS__)); \
		exit(ret);                                     \
	} while (0)

typedef unsigned long long ull;


int main(int argc, char** argv) {
	// get the current time to set as the seed
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	srandom(ts.tv_nsec);

	// return the result if no input
	if (argc <= 1)
		return printf("%s\n", (rand() & 1) ? "heads" : "tails");

	// loop through arguments
	for (unsigned i = 1; i < (unsigned)argc; ++i) {
		// parse the argument
		errno = 0;
		ull c = strtoull(argv[i], NULL, 10);
		if (errno != 0) error(errno, "parse error for string: '%s'\n", argv[i]);

		// perform for the input count
		ull headsc = 0; // amount of heads
		ull tailsc = 0; // amount of tails

		while (c >= 31) {
			c -= 31;
			long n = random();                // generate a random number for this batch
			int cnt = __builtin_popcountl(n); // counts the set bits
			headsc += cnt;
			tailsc += 31 - cnt;
		}

		if (c > 0) {
			long n = random();
			long msk = (1 << c) - 1;
			int cnt = __builtin_popcountl(n & msk);
			headsc += cnt;
			tailsc += c - cnt;
		}

		// print the results of this cycle
		printf("results:\n heads: %llu\n tails: %llu\n", headsc, tailsc);
	}
	return 0;
}
