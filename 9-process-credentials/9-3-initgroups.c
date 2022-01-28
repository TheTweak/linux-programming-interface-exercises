#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SG_SIZE (NGROUPS_MAX + 1)

int initgroups_(const char *user, gid_t group) {
	gid_t groups[SG_SIZE];
	int numGroups = 0;
	// retrieve user group from /etc/password file
	struct passwd *pwd = getpwnam(user);
	groups[numGroups++] = pwd->pw_gid;

	// retrieve all groups a user is member of from /etc/groups file
	setgrent();
	struct group *grp;
	while ((grp = getgrent()) != NULL) {
		char **groupMember = grp->gr_mem;
		while (*groupMember != NULL) {
			if (strcmp(user, *groupMember) == 0) {
				printf("group %s[%d] member %s\n", grp->gr_name, grp->gr_gid, *groupMember);
				groups[numGroups++] = grp->gr_gid;
			}
			groupMember += 1;
		}
	}
	endgrent();
	groups[numGroups++] = group;
	for (int i = 0; i < numGroups; i++) {
		printf("group id: %d\n", groups[i]);
	}
	setgroups(numGroups, groups);
}

int main(int argc, char *argv[]) {
	initgroups_("thetweak", 1337);
	gid_t groups[SG_SIZE];
	int groupsNum = getgroups(SG_SIZE, groups);
	printf("getgroups result: \n");
	for (int i = 0; i < groupsNum; i++) {
		printf("group id: %d\n", groups[i]);
	}
}
