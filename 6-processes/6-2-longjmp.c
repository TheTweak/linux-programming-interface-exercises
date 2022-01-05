#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

void foo() {
	switch(setjmp(env)) {
		case 0:
			printf("setup jump returned 0");
			break;
			
		case 1:
			printf("returned to foo again");
			break;
	}
}

int main(int argc, char* argv[]) {
	foo();
	longjmp(env, 1);
	exit(EXIT_SUCCESS);
}
