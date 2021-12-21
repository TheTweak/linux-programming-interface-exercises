#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {
	char *fileName = argv[1];
	int numBytes = atoi(argv[2]);
	int skipAppend = 0;
	if (argc == 4) {
		skipAppend = 1;
	}
	printf("%s %d %d", fileName, numBytes, skipAppend);

	int flags = O_WRONLY | O_CREAT;
	if (skipAppend == 0) {
		flags |= O_APPEND;
	}

	int fd = open(fileName, flags, S_IRWXU);
	if (fd == -1) {
		errExit("opening file %s", fileName);
	}

	char bytes[numBytes];
	int totalWrite = 0;
	for (int i = 0; i < numBytes; i++) {
		if (skipAppend && (lseek(fd, 0, SEEK_END) == -1)) {
			errExit("lseek");	
		}
		totalWrite += write(fd, bytes, 1);	
	}
	
	if (totalWrite != numBytes) {
		errExit("writing to file");
	}
	
	if (close(fd) == -1) {
		errExit("closing file");
	}

	exit(0);
}
