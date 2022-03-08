#include <pwd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

ssize_t readFile(int fd, char **dest, int destSize) {
	ssize_t bufferSize = 128;
	char buffer[bufferSize];  
	char *ptr = *dest;
	ssize_t size = destSize;
	ssize_t readBytes = 0;
	ssize_t totalRead = 0;
	while ((readBytes = read(fd, buffer, bufferSize)) != 0) {
		totalRead += readBytes;
		if (totalRead > size) {
			char *newLineBytes = malloc(size*2);
			memcpy(newLineBytes, *dest, size);
			dest = &newLineBytes;
			ptr = *dest + size;
			size = size*2;
		}
		memcpy(ptr, &buffer, readBytes);
		ptr += readBytes;
	}
	return totalRead;
}

char* constructStatusDirPath(struct dirent *dirEnt) {
	const char *statusDir = "/proc/status";
	const char* dirName = dirEnt->d_name;
	int arraySize = strlen(statusDir) + strlen(dirName) + 2;
	char *statusDirName = malloc(arraySize);
	for (int i = 0; i < arraySize; i++) {
		statusDirName[i] = '\0';
	}
	strcat(statusDirName, "/proc/");
	strcat(statusDirName, dirName);
	strcat(statusDirName, "/status");
	return statusDirName;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: ps <user-name>\n");
		return 0;
	}

	const char *userName = argv[1];
	struct passwd *pwd = getpwnam(userName);
	if (pwd == NULL) {
		printf("Failed to get password record for user %s\n", userName);
		return 1;
	}
	printf("UID: %d\n", pwd->pw_uid);

	DIR *procDir = opendir("/proc");
	if (procDir == NULL) {
		printf("Failed to opendir\n");
		return 1;	
	}
	
	struct dirent *dirEnt;
	while ((dirEnt = readdir(procDir)) != NULL) {
		char *statusDirName = constructStatusDirPath(dirEnt);
		int fd;
		if ((fd = open(statusDirName, O_RDONLY)) != -1) {
			printf("\n\nstatus dir: %s \n", statusDirName);
			int initialSize = 512;
			char *content = malloc(initialSize);
			ssize_t totalRead = readFile(fd, &content, initialSize);
			//write(1, content, totalRead);
			close(fd);
			write(1, content+0, 20);
			/*
			ssize_t l = 0;
			ssize_t r = 0;
			while (r < totalRead) {
				while (r < totalRead && *(content+r) != '\n') {
					r++;
				}
				write(1, content + l, r);
				l = r;
			}*/
			break;
		}
	}
}
