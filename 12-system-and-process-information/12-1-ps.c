#include <pwd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: ps <user-name>\n");
		return 0;
	}

	const char *userName = argv[1];
	struct passwd *pwd = getpwnam(userName);
	if (pwd == NULL)
	{
		printf("Failed to get password record for user %s\n", userName);
		return 1;
	}
	printf("UID: %d\n", pwd->pw_uid);

	DIR *procDir = opendir("/proc");
	if (procDir == NULL)
	{
		printf("Failed to opendir\n");
		return 1;	
	}
	
	struct dirent *dirEnt;
	const char *statusDir = "/proc/status";
	while ((dirEnt = readdir(procDir)) != NULL)
	{
		const char* dirName = dirEnt->d_name;
		int arraySize = strlen(statusDir) + strlen(dirName) + 2;
		char statusDirName[arraySize];
		for (int i = 0; i < arraySize; i++) {
			statusDirName[i] = '\0';
		}
		strcat(statusDirName, "/proc/");
		strcat(statusDirName, dirName);
		strcat(statusDirName, "/status");
		int fd;
		if ((fd = open(statusDirName, O_RDONLY)) != -1) {
			printf("status dir: %s \n", statusDirName);	
		}
	}
}
