#ifndef PAGETABLE_H
#define PAGETABLE_H

#define PAGE_TABLE_ENTRIES 32
#define PAGE_SIZE 128
#define FRAME_SIZE 128

void init_page_table();
int get_frame(int page_number);
void set_frame(int page_number, int frame_number);
int get_page_frame(int page_number);
void invalidate_page(int page_number);

#endif
