#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <stdio.h>

struct passwd *getpwnam(const char *name) {
	setpwent();
	struct passwd *pwd = getpwent();
	for (; pwd != NULL; pwd = getpwent()) {
		if (strcmp(name, pwd->pw_name) == 0) {
			break;
		}
	}
	endpwent();
	return pwd;
}

int main(int argc, char *argv[]) {
	struct passwd *pwd = getpwnam("root");
	printf("user id is %d\n", pwd->pw_uid);
	pwd = getpwnam("games");
	printf("user id is %d\n", pwd->pw_uid);
}
