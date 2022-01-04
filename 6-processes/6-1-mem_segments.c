#include <stdlib.h>

char globBuf[65536]; /* uninitialized data segment */
int primes[] = {2, 3, 5, 7}; /* init data segment */

static int square(int x) { /* allocated in frame for square() */
	int result;
	result = x * x;
	return result; /* return value passed via register */
}

int main(int argc, char *argv[]) {
	static int key = 9973; /* init data segment */
	static char mbuf[1024]; /* uninit data segment */
	static char sbuf[10] = "abcdeabcde";
	for (size_t i = 0; i < sizeof(mbuf)/sizeof(mbuf[0]); i++) {
		mbuf[i] = 'a';
	}
	char *p;
	p = malloc(1024);
	exit(EXIT_SUCCESS);
}
