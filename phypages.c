#include "phypages.h"
#include <stdlib.h>

int frames[FRAME_COUNT] = {0}; // 0 indicates free, 1 indicates used

int allocate_frame() {
	int i;
	/*
	 * Start from 1 to reserve frame 0
	 * for OS.
	*/
	for (i = 1; i < FRAME_COUNT; i++) {
		if (frames[i] == 0) {
			frames[i] = 1;
			return i;
		}
	}
	return -1; // No free frame
}

void free_frame(int frame_number) {
	frames[frame_number] = 0;
}
