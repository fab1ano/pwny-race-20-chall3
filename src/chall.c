#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>
#include <stdbool.h>


#define PORT 4444
#define STORAGE_SIZE 2021

char *pwny = "\n"
"                .''\n"
"      ._.-.___.' (`\\\n"
"     //(        ( `'\n"
"    '/ )\\ ).__. ) \n"
"    ' <' `\\ ._/'\\\n"
"       `   \\     \\\n";


bool init = false;
static char buf[1024];

uint32_t read_uint() {
	if (fgets(buf, 1024, stdin) == NULL) {
		err(-1, "fgets");
	}
	uint32_t result = strtoull(buf, NULL, 10);
	memset(buf, 0, sizeof(buf));
	return result;
}

uint64_t read_ulong() {
	if (fgets(buf, 1024, stdin) == NULL) {
		err(-1, "fgets");
	}
	uint64_t result = strtoull(buf, NULL, 10);
	memset(buf, 0, sizeof(buf));
	return result;
}


void edit_entry(uint64_t *storage, uint32_t size) {
	int32_t index = 0;
	int64_t value = 0;

	puts("Index: ");
	index = read_uint();
	if (index >= (int) size) {  // no array index out of bounds
		puts("We are not yet in 2021!");
		exit(EXIT_FAILURE);
	}

	puts("Value: ");
	value = read_ulong();

	storage[index] = value;
}

void show_entry(uint64_t *storage, uint32_t size) {
	int32_t index = 0;

	puts("Index: ");
	index = read_uint();
	if (index >= (int) size) {  // no array index out of bounds
		puts("We are not yet in 2021!");
		exit(EXIT_FAILURE);
	}

	printf("Current Value: %ld\n", storage[index]);
}

void print_storage(uint64_t *storage, uint32_t size) {
	printf("Storage:\n");
	for (int32_t i = 0; i<size; i++) {
		if (storage[i]) {
			printf("%d: %ld\n", i, storage[i]);
		}
	}
}


void main_loop() {
	uint64_t storage[STORAGE_SIZE];
	int32_t cmd = -1;
	bool loop = true;

	memset(storage, 0, sizeof(storage));

	while (loop) {
		puts("What do you wanna do?");
		puts("  0: print storage");
		puts("  1: edit entry");
		puts("  2: show entry");
		puts("  3: exit");

		cmd = read_uint();
		switch (cmd) {
			case 0: print_storage(storage, STORAGE_SIZE); break;
			case 1: edit_entry(storage, STORAGE_SIZE); break;
			case 2: show_entry(storage, STORAGE_SIZE); break;
			default:
				loop = false;
				break;
		}
	}

	puts("Bye!");
	exit(EXIT_SUCCESS);
}


int main(void) {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	alarm(60);

	puts(pwny);

	main_loop();
}
