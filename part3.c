#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int *page_table;
int *last_access_time;
int current_time = 0;

void init_page_table(int page_table_entries) {
	page_table = malloc(page_table_entries * sizeof(int));
	last_access_time = malloc(page_table_entries * sizeof(int));
	int i;

	for (i = 0; i < page_table_entries; i++) {
		page_table[i] = -1;
		last_access_time[i] = 0;
	}
}

void free_page_table() {
	free(page_table);
	free(last_access_time);
}

int perform_lru_replacement(int page_table_entries) {
	int lru_page = 0;
	int min_time = last_access_time[0];
	int i;

	for (i = 1; i < page_table_entries; i++) {
		if (page_table[i] != -1 && last_access_time[i] < min_time) {
			min_time = last_access_time[i];
			lru_page = i;
		}
	}

	int frame_to_free = page_table[lru_page];
	page_table[lru_page] = -1;

	return frame_to_free;
}

void translate_address(uint64_t logical_addr, int bytes_per_page, int page_table_entries, int frame_count, FILE *outFile) {
	int page_number = logical_addr / bytes_per_page;
	int offset = logical_addr % bytes_per_page;

	current_time++;

	int frame_number = page_table[page_number];
	if (frame_number == -1) {
		frame_number = page_number % frame_count;
		if (frame_number == -1) {
			frame_number = perform_lru_replacement(page_table_entries);
		}
		page_table[page_number] = frame_number;
	}

	last_access_time[page_number] = current_time;

	uint64_t physical_addr = (frame_number * bytes_per_page) + offset;
	fwrite(&physical_addr, sizeof(uint64_t), 1, outFile);
}

int main(int argc, char *argv[]) {
	if (argc != 6) {
		fprintf(stderr, "Usage: %s <BytesPerPage> <SizeOfVirtualMemory> <SizeOfPhysicalMemory> <SequenceFile> <OutputFile>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int bytes_per_page = atoi(argv[1]);
	int virtual_memory_size = atoi(argv[2]);
	int physical_memory_size = atoi(argv[3]);
	int page_table_entries = virtual_memory_size / bytes_per_page;
	int frame_count = physical_memory_size / bytes_per_page;

	init_page_table(page_table_entries);

	FILE *inFile = fopen(argv[4], "rb");
	FILE *outFile = fopen(argv[5], "wb");
	if (!inFile || !outFile) {
		perror("File opening failed");
		return EXIT_FAILURE;
	}

	uint64_t logical_addr;
	while (fread(&logical_addr, sizeof(uint64_t), 1, inFile) == 1) {
		translate_address(logical_addr, bytes_per_page, page_table_entries, frame_count, outFile);
	}

	fclose(inFile);
	fclose(outFile);

	free_page_table();
	return EXIT_SUCCESS;
}
