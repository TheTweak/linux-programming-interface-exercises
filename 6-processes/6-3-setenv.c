#include <stdlib.h>
#include <string.h>

#include "tlpi_hdr.h"

extern char **environ;

int setenv(const char *name, const char *value, int overwrite) {
	char *existingValue = getenv(name);
	if (existingValue == NULL || overwrite) {
		char *nameValue = malloc(strlen(name) + strlen(value) + 2);
		strcat(nameValue, name);
		strcat(nameValue, "=");
		strcat(nameValue, value);
		putenv(nameValue);
	}
	return 0;
}

int unsetenv(const char *name) {
	char **p = environ;
	bool deleted = 0; 
	for (; *p != NULL; p++) {
		char *equalSignPtr = strchr(*p, '=');
		size_t equalSignIndex = equalSignPtr - *p;
		char *pName = strndup(*p, equalSignIndex);
		if (strcmp(pName, name) == 0) {
			printf("pName is %s (%s), removing %s\n", pName, *p, name);
			deleted = 1;
			break;
		}
	}
	if (deleted) {
		for (; *p != NULL; p++) {
			*p = *(p + 1);
		}
	}
	return 0;
}

void printEnv() {
	printf("\n");
	for (char **p = environ; *p != NULL; p++) {
		printf("%s\n", *p);
	}	
}

int main(int argc, char *argv[]) {
	setenv("qwer", "asd", 0);
	if (strcmp(getenv("qwer"), "asd") != 0) {
		errExit("setenv without overwrite and new value");
	}
	setenv("qwer", "ghjk", 0);
	if (strcmp(getenv("qwer"), "asd") != 0) {
		errExit("setenv without overwrite");
	}
	setenv("qwer", "ghjk", 1);
	if (strcmp(getenv("qwer"), "ghjk") != 0) {
		errExit("setenv with overwrite and new value");
	}
	
	setenv("qwerty", "value", 0);
	printEnv();
	unsetenv("qwer");
	printf("\n*********After unset(qwer)\n");
	printEnv();
	if (getenv("qwer") != NULL) {
		errExit("Unset env single value");
	}
	setenv("asd", "qwe", 0);
	setenv("asd", "hjk", 0);
	printf("\n*********Before unset(asd)\n");
	printEnv();
	unsetenv("asd");
	printf("\n*********After unset(asd)\n");
	printEnv();
	if (getenv("asd") != NULL) {
		errExit("Unset env multiple values");
	}
	char *qwerty = getenv("qwerty");
	printf("qwerty value is %s\n", qwerty);
	if (qwerty == NULL ||  strcmp(getenv("qwerty"), "value") != 0) {
		errExit("Unset removed more than needed");
	}
}
