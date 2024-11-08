#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PAGE_SIZE          128
#define FRAME_SIZE         128
#define PAGE_TABLE_ENTRIES 32

int page_table[PAGE_TABLE_ENTRIES] = {
	2, 4, 7, 3, 1, 5, 6, 0, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

void translate_address(uint64_t logical_addr, FILE *outFile) {
	uint64_t page_number = (logical_addr / PAGE_SIZE) % PAGE_TABLE_ENTRIES;
	uint64_t offset = logical_addr % PAGE_SIZE;

	int frame_number = page_table[page_number];
	if (frame_number == -1) {
		fprintf(stderr, "Invalid page reference.\n");
		exit(EXIT_FAILURE);
	}

	uint64_t physical_addr = (frame_number * FRAME_SIZE) + offset;
	fwrite(&physical_addr, sizeof(uint64_t), 1, outFile);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <inFile> <outFile>\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *inFile = fopen(argv[1], "rb");
	FILE *outFile = fopen(argv[2], "wb");
	if (!inFile || !outFile) {
		perror("File opening failed");
		return EXIT_FAILURE;
	}

	uint64_t logical_addr;
	while (fread(&logical_addr, sizeof(uint64_t), 1, inFile) == 1) {
		translate_address(logical_addr, outFile);
	}

	fclose(inFile);
	fclose(outFile);
	return EXIT_SUCCESS;
}
