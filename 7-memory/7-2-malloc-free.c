#include <unistd.h>
#include <stdio.h>
#include <string.h>

void *malloc(size_t size) {
	size_t *ptr = sbrk(size + sizeof(size_t));
	*ptr = size;
	return ptr + 1; 
}

void *freeHead;
void *freeTail;

void free(void *ptr) {
	size_t *sizePtr = (size_t*) ptr;
	sizePtr -= 1;
	printf("block size is: %zu\n", *sizePtr);
	if (freeHead == NULL) {
		freeHead = ptr;
		freeTail = ptr;
		return;
	}

	int **nextPtr = (int**)&ptr;
	((int**)freeHead)[0] = *nextPtr;
	printf("ptr is %10p, head is %10p, head.next is %10p\n", ptr, *((int**)freeHead), *nextPtr);
	freeTail = ptr;
}

int main(int argc, char *argv[]) {
	printf("size of void* = %zu, size of int* = %zu\n", sizeof(void*), sizeof(int*));
	char *strPtr = malloc(sizeof(char)*20);
	strcpy(strPtr, "Hello, world\n");
	printf("%s", strPtr);
	free(strPtr);

	strPtr = malloc(sizeof(char)*10);
	strcpy(strPtr, "test\n");
	free(strPtr);

	printf("freed blocks:\n");
	int i = 0;
	void *ptr = freeHead;
	while (i < 2) {
		printf("freed ptr is %10p\n", ptr);
		size_t *sizePtr = (size_t*) ptr;
		sizePtr -= 1;
		printf("size: %zu\n", *sizePtr);
		printf("next is %10p\n", *((int**)ptr));
		ptr = *(int**)ptr;
		i += 1;
	}
}
