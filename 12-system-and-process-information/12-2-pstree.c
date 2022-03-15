#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

typedef struct {
	int id;
	int parentId;
	char *name;
} Process;

void printIdent(int level) {
	for (int i = 0; i < level; i++) {
		printf(" ");
	}	
}

static int cmpProcess(const void *p1, const void *p2) {
	return ((const Process *)p1)->id - ((const Process *)p2)->id;
}

void printChildren(int level, Process *proc, Process *procs, size_t procCount) {
	printIdent(level);
	printf("[%d] %s\n", proc->id, proc->name);
	Process *p = procs;
	for (size_t i = 0; i < procCount; i++) {
		if (p->parentId == proc->id) {
			printChildren(level + 2, p, procs, procCount);
		}
		p++;
	}
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

char *readValue(char *key, char* content) {
	char *keyColon = malloc(strlen(key) + 2);
	strcpy(keyColon, key);
	strcat(keyColon, ":");
	char *keyStart;
	if ((keyStart = strstr(content, keyColon)) != NULL) {
		char *uid = strtok(keyStart, "\t");
		uid = strtok(NULL, "\n");
		return uid;
	}
}

Process *getProcesses(size_t *countResult) {
	DIR *procDir = opendir("/proc");
	if (procDir == NULL) {
		printf("Failed to opendir\n");
		return NULL;	
	}

	size_t procCount = 5;
	Process *result = malloc(sizeof(Process)*procCount);
	Process *ptr = result;
	struct dirent *dirEnt;
	size_t i = 0;
	while ((dirEnt = readdir(procDir)) != NULL) {
		char *statusDirName = constructStatusDirPath(dirEnt);
		int fd;
		if ((fd = open(statusDirName, O_RDONLY)) != -1) {
			int initialSize = 512;
			char *content = malloc(initialSize);
			readFile(fd, &content, initialSize);
			close(fd);

			if (i >= procCount) {
				Process *newProc = malloc(sizeof(Process)*procCount*2);
				memcpy(newProc, result, sizeof(Process)*procCount);
				result = newProc;
				procCount *= 2;
				ptr = result + i;
			}
			char *pprocId = readValue("PPid", content);
			char *procId = readValue("Pid", content);
			char *procName = readValue("Name", content);
			Process proc = {.id=atoi(procId), .parentId=atoi(pprocId), .name=procName};
			*ptr = proc;
			i++;
			ptr++;
		}
	}
	*countResult = i;
	return result;
}

int main(int argc, char *argv[]) {
	size_t count;
	Process *procs = getProcesses(&count);
	if (procs == NULL) {
		return 1;
	}
	qsort(procs, count, sizeof(Process), cmpProcess);
	printChildren(0, procs, procs, count);
}
