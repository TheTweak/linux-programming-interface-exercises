#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


int main(int argc, char* argv[]) {
	if (argc != 2) {
		errExit("only one argument is supported - file name");
	}

	char *fileName = argv[1];

	int statusFlags = S_IRWXU | S_IRGRP;
	int fd = open(fileName, O_CREAT | O_RDWR, statusFlags);
	if (fd == -1) {
		errExit("creating file %s", fileName);
	}

	int fdDup = dup(fd);
	if (fdDup == -1) {
		errExit("duplicating fd");
	}

	int dupFileStatusFlags = fcntl(fdDup, F_GETFL);
	int fileStatusFlags = fcntl(fd, F_GETFL);
	
	printf("dup fd status flags: %d, original fd status flags: %d\n", dupFileStatusFlags, fileStatusFlags);
	if (fileStatusFlags != dupFileStatusFlags) {
		errExit("file status flags are not equal");
	}

	char data[] = "hello world\n";
	int writeBytes = write(fd, data, strlen(data));
	if (writeBytes != strlen(data)) {
		errExit("writing to file");
	}

	int dupOffset = lseek(fdDup, 0, SEEK_CUR);
	int offset = lseek(fd, 0, SEEK_CUR);
	printf("dup fd offset: %d, original fd offset: %d\n", dupOffset, offset);
	if (dupOffset != offset) {
		errExit("offsets are not equal");
	}

	close(fd);
	close(fdDup);
}
