/*
Excercise 5-2 from page 110.
*/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
	int fd = open(argv[1], O_WRONLY | O_APPEND);
	if (fd == -1) {
		errExit("opening file %s", argv[1]);
	}
	
	int seek = lseek(fd, 0, SEEK_SET);
	if (seek == -1) {
		errExit("seeking offset %s", argv[1]);
	}
	
	char text[] = "hello\n";
	ssize_t numWrite = write(fd, text, strlen(text));
	if (numWrite != strlen(text)) {
		errExit("writing to file");
	}

	if (close(fd) == -1) {
		errExit("closing the file");
	}
}
