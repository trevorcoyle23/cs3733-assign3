#include "pagetable.h"
#include <stdlib.h>

int page_table[PAGE_TABLE_ENTRIES] = {0};

void init_page_table() {
	int i;
	for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		page_table[i] = -1;
	}
}

int get_frame(int page_number) {
	return page_table[page_number];
}

void set_frame(int page_number, int frame_number) {
	page_table[page_number] = frame_number;
}

int get_page_frame(int page_number) {
	return page_table[page_number];
}

void invalidate_page(int page_number) {
	page_table[page_number] = -1;
}
