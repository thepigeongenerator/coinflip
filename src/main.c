#include <assert.h>
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

typedef struct dynrdat {
	size_t cap;
	ull dat[];
} dynrdat;

dynrdat* rdat = NULL;

static inline ull pow2_ceil(ull x) {
	x -= !!x;                        // if x=0, remains 0; else x -= 1
	int lz = __builtin_clzll(x | 1); // get leading zeroes
	return (~0ULL >> lz) + 1;        // bit-shift the maximum value by this amount of leading zeroes
}

static void quit(void) {
	free(rdat);
}

int main(int argc, char** argv) {
	// return the result if no input
	if (argc <= 1) return printf("%s\n", (clock() & 1) ? "heads" : "tails");
	atexit(quit);

	// loop through arguments
	for (unsigned i = 1; i < (unsigned)argc; ++i) {
		// parse the argument
		errno = 0;
		ull c = strtoull(argv[i], NULL, 10);
		if (errno != 0) error(errno, "parse error for string: '%s'\n", argv[i]);

		// acquire random data (compiler will optimize MOD and DIV away since they're both base-2 constant values)
		int mod = c % (sizeof(ull) * 8); // get the remainder of the available random bits
		c = c / (sizeof(ull) * 8);       // compute our "word count"

		// dynamically scale the array to our needs, ensuring 2^n scaling
		size_t cap = pow2_ceil(c + !!mod);
		if (!rdat || rdat->cap < cap) {
			void* ptr = realloc(rdat, sizeof(dynrdat) + sizeof(ull) * cap);
			if (!ptr) error(1, "insufficient memory\n", );

			rdat = ptr;
			rdat->cap = cap;
		}

		// populate the array with random data
		if (getrandom(rdat->dat, (c + !!mod) * sizeof(ull), 0) < 0)
			error(1, "failed to acquire random data!\n", );

		// perform for the input count
		ull headsc = 0; // amount of heads
		ull tailsc = 0; // amount of tails

		// loop through how many full words there are available
		ull* n = rdat->dat; // initialize the pointer to the start of our random data
		for (; n < (rdat->dat + c); n++) {
			int cnt = __builtin_popcountll(*n); // counts the set bits
			headsc += cnt;
			tailsc += sizeof(ull) * 8 - cnt;
		}

		// if there is a remainder, use the last N to get this
		if (mod) {
			ull msk = (1 << mod) - 1; // get a mask with the set word size
			int cnt = __builtin_popcountll(*n & msk);
			headsc += cnt;
			tailsc += mod - cnt;
		}

		// print the results of this cycle
		printf("results:\n heads: %llu\n tails: %llu\n", headsc, tailsc);
	}

	return 0;
}
