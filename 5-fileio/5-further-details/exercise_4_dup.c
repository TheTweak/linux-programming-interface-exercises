#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


int dup(int oldfd) {
	int r = fcntl(oldfd, F_DUPFD, 0);

	if (r == -1) {
		errExit("fcntl");
	}

	return r;
}

int dup2(int oldFd, int newFd) {
	if (oldFd == newFd) {
		// check if oldFd is valid
		if (fcntl(oldFd, F_GETFL) == -1) {
			errExit("invalid oldFd");
		} else {
			return newFd;
		}
	}

	if (close(newFd) == -1) {
		errExit("closing newFd");
	}

	int r = fcntl(oldFd, F_DUPFD, newFd);

	if (r != newFd) {
		errExit("r=%d not equal to newFd=%d", r, newFd);
	}
	
	return r;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		errExit("filename argument is not provided");
	} 

	int r = open(argv[1], O_RDWR | O_CREAT, S_IRWXU);

	if (r == -1) {
		errExit("opening file %s", argv[1]);
	}
	printf("original fd=%d\n", r);

	close(2); // close stderr to demonstrate dup
	int rd = dup(r);
	if (rd != 2) {
		errExit("did not reuse lowest available fd");
	}
	printf("duplicated fd=%d\n", rd);

	char data[] = "hello world\n";
	int numWrite = write(r, data, strlen(data));
	if (numWrite != strlen(data)) {
		errExit("writing to r");
	}

	numWrite = write(rd, data, strlen(data));
	if (numWrite != strlen(data)) {
		errExit("writing to rd");
	}

	int rd2 = dup2(r, rd);
	
	numWrite = write(rd2, data, strlen(data));
	if (numWrite != strlen(data)) {
		errExit("writing to rd2");
	}

	int r_same = dup2(r, r);
	if (r_same != r) {
		errExit("dup2 with same oldFd and newFd");
	}

	close(rd);
	close(r);
	close(rd2);
}
