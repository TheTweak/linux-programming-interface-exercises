#include <sys/uio.h>

ssize_t readv(int fd, const struct iovec *iov, int iovcnt) {
	
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt) {
	
}

int main(int argc, char *argv[]) {
	int fd = open("exercise_5_7.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);

	if (fd == -1) {
		errExit("creating file");
	}

	int iovcnt = 5;
	struct iovec iov = /* allocate */;

	ssize_t bytesWrite = writev(fd, &iov, iovcnt);

	int iovcntr = 5;
	struct iovec iovr = /* allocate */;

	ssize_t bytesRead = readv(fs, &iovr, iovcntr);

	if (bytesRead != bytesWrite) {
		errExit("bytesRead not equal to bytesWrite");
	}

	close(fd);
}
