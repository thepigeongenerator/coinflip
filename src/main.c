#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <time.h>

#define error(ret, s, ...)                             \
	do {                                               \
		fprintf(stderr, (s)__VA_OPT__(, __VA_ARGS__)); \
		exit(ret);                                     \
	} while (0)

typedef unsigned long long ull;


int main(int argc, char** argv) {
	// return the result if no input
	if (argc <= 1) return printf("%s\n", (rand() & 1) ? "heads" : "tails");

	// loop through arguments
	for (unsigned i = 1; i < (unsigned)argc; ++i) {
		// parse the argument
		errno = 0;
		ull c = strtoull(argv[i], NULL, 10);
		if (errno != 0) error(errno, "parse error for string: '%s'\n", argv[i]);

		// perform for the input count
		ull headsc = 0; // amount of heads
		ull tailsc = 0; // amount of tails

		while (c >= (sizeof(ull) * 8)) {
			c -= (sizeof(ull) * 8);
			ull n;
			getrandom(&n, sizeof(ull), GRND_INSECURE);
			int cnt = __builtin_popcountll(n); // counts the set bits
			headsc += cnt;
			tailsc += 31 - cnt;
		}

		if (c > 0) {
			long n;
			getrandom(&n, sizeof(ull), GRND_INSECURE);
			long msk = (1 << c) - 1;
			int cnt = __builtin_popcountll(n & msk);
			headsc += cnt;
			tailsc += c - cnt;
		}

		// print the results of this cycle
		printf("results:\n heads: %llu\n tails: %llu\n", headsc, tailsc);
	}
	return 0;
}
