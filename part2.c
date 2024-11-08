#include "pagetable.h"
#include "phypages.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PAGE_TABLE_ENTRIES 32

int last_access_time[PAGE_TABLE_ENTRIES] = {0};
int current_time = 0;
int page_fault_count = 0;

int perform_lru_replacement() {
	int lru_page = 0;
	int min_time = last_access_time[0];
	int i;

	for (i = 1; i < PAGE_TABLE_ENTRIES; i++) {
		if (get_page_frame(i) != -1 && last_access_time[i] < min_time) {
			min_time = last_access_time[i];
			lru_page = i;
		}
	}

	int frame_to_free = get_page_frame(lru_page);
	free_frame(frame_to_free);

	invalidate_page(lru_page);

	return frame_to_free;
}

void translate_and_output(FILE *inFile, FILE *outFile) {
	uint64_t logical_addr;
	while (fread(&logical_addr, sizeof(uint64_t), 1, inFile) == 1) {
		int page_number = (logical_addr / PAGE_SIZE) % PAGE_TABLE_ENTRIES;
		int offset = logical_addr % PAGE_SIZE;

		current_time++;

		int frame_number = get_frame(page_number);
		if (frame_number == -1) {
			page_fault_count++;
			frame_number = allocate_frame();
			if (frame_number == -1) {
				frame_number = perform_lru_replacement();
			}
			set_frame(page_number, frame_number);
		}

		last_access_time[page_number] = current_time;

		uint64_t physical_addr = (frame_number * FRAME_SIZE) + offset;
		fwrite(&physical_addr, sizeof(uint64_t), 1, outFile);
	}
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

	translate_and_output(inFile, outFile);

	fclose(inFile);
	fclose(outFile);
	return EXIT_SUCCESS;
}
