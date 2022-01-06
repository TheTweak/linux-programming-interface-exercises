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
}
