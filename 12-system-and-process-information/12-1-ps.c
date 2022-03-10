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
			int initialSize = 512;
			char *content = malloc(initialSize);
			readFile(fd, &content, initialSize);
			close(fd);

			char *uidStart;
			if ((uidStart = strstr(content, "Uid:")) != NULL) {
				char *uid = strtok(uidStart, "\t");
				uid = strtok(NULL, "\t");
				if (atoi(uid) == (int)pwd->pw_uid) {
					write(1, content, strchr(content, '\n') - content);
					write(1, "\n", 1);
				}
			}
		}
	}
}
