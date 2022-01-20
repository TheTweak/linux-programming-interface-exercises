#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	uid_t uid1 = 5;
	uid_t uid2 = 1;
	struct passwd *pwd = getpwuid(uid1);
	size_t userNameLength = 0;
	char *ptr = pwd->pw_name;
	while (*ptr != '\0') {
		userNameLength++;
		ptr += 1;
	}
	char *user1 = malloc(userNameLength);
	memcpy(user1, pwd->pw_name, userNameLength);
	pwd = getpwuid(uid2);
	printf("%s %s\n", user1, pwd->pw_name);
}
