#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
	int numRead;
	char buf[BUF_SIZE];
	int outputFd = -1;
	if (argc > 1) {
		int openFlags = O_CREAT | O_WRONLY;
		int appendMode = getopt(argc, argv, ":a");
		if (appendMode != -1) {
			openFlags |= O_APPEND;
		} else {
			openFlags |= O_TRUNC;
		}
		outputFd = open(argv[optind], openFlags);
	}
	while ((numRead = read(0, buf, BUF_SIZE)) > 0) {
		if (write(1, buf, numRead) != numRead) {
			fatal("write() to stdout failed or partial write occurred");
		}
		if (outputFd != -1 && write(outputFd, buf, numRead) != numRead) {
			fatal("write() to %s failed or partial write occurred", argv[optind]);
		}
	}

	if (outputFd != -1 && (close(outputFd) == -1)) {
		errExit("close output");
	}

	exit(EXIT_SUCCESS);
}