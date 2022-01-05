#include <sys/uio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "tlpi_hdr.h"

ssize_t readv(int fd, const struct iovec *iov, int iovcnt) {
	ssize_t total = 0;
	for (int i = 0; i < iovcnt; i++) {
		ssize_t bytes = read(fd, iov[i].iov_base, iov[i].iov_len);
		total += bytes;
		if (bytes != iov[i].iov_len) {
			errExit("readv bytes read mismatch");
		}
	}
	return total;
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt) {
	ssize_t total = 0;
	for (int i = 0; i < iovcnt; i++) {
		ssize_t bytesw = write(fd, iov[i].iov_base, iov[i].iov_len);
		total += bytesw;
		if (bytesw != iov[i].iov_len) {
			errExit("writev bytes written mismatch");
		}
	}
	return total;
}

int main(int argc, char *argv[]) {
	int fd = open("exercise_5_7.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);

	if (fd == -1) {
		errExit("creating file");
	}

	int iovCnt = 3;
	struct iovec writeIovecArray[iovCnt];
	char dataString[] = "Hello World!\n";
	for (int i = 0; i < iovCnt; i++) {
		writeIovecArray[i].iov_base = strdup(dataString);
		writeIovecArray[i].iov_len = strlen(dataString) + 1;
	}

	ssize_t bytesWrite = writev(fd, writeIovecArray, iovCnt);
	printf("bytes write: %ld\n", bytesWrite);
	if (bytesWrite == 0) {
		errExit("writing to file");
	}
	
	for (int i = 0; i < iovCnt; i++) {
		free(writeIovecArray[i].iov_base);
	}

	fd = open("exercise_5_7.txt", O_RDONLY);

	if (fd == -1) {
		errExit("opening the file");
	}
	
	struct iovec iovr[iovCnt];
	for (int i = 0; i < iovCnt; i++) {
		iovr[i].iov_base = malloc(strlen(dataString) + 1);
		iovr[i].iov_len = strlen(dataString) + 1;
	}
	ssize_t bytesRead = readv(fd, iovr, iovCnt);

	for (int i = 0; i < iovCnt; i++) {
		free(iovr[i].iov_base);
	}

	if (bytesRead != bytesWrite) {
		errExit("bytesRead=%d not equal to bytesWrite=%d",
			bytesRead, bytesWrite);
	}

	close(fd);
}
