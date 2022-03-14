#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char *argv[]) {
	Process procsArr[] = {
		{ .id = 0, .parentId = -1, .name = "init" },
		{ .id = 1, .parentId = 0, .name = "ls" },
		{ .id = 2, .parentId = 0, .name = "java" },
		{ .id = 3, .parentId = 2, .name = "thread0" },
		{ .id = 4, .parentId = 2, .name = "thread1" },
	};

	size_t count = sizeof(procsArr)/sizeof(Process);
	Process *procs = malloc(sizeof(Process)*count);
	Process *p = procs;
	for (size_t i = 0; i < count; i++) {
		p->id = procsArr[i].id;
		p->parentId = procsArr[i].parentId;
		p->name = procsArr[i].name;
		p++;
	}

	printChildren(0, procs, procs, count);

	free(procs);
}
