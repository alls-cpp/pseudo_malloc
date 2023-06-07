#include <stdio.h>

#include "buddy_allocator.h"

#define PAGE_SIZE 4096

#define BUDDY_ALLOCATOR_MEMORY_SIZE (1 << 20) // 1 MB
// number of levels should be grater than 10, because the allocator
// could not allocate a block of size greater than 1/4 of the page size
#define BUDDY_ALLOCATOR_LEVELS 5
#define BUDDY_ALLOCATOR_MIN_BUCKET_SIZE (BUDDY_ALLOCATOR_MEMORY_SIZE >> (BUDDY_ALLOCATOR_LEVELS - 1))
#define BITMAP_BUFFER_SIZE (1 << (BUDDY_ALLOCATOR_LEVELS + 1))

char memory[BUDDY_ALLOCATOR_MEMORY_SIZE];
uint8_t bitmap_buffer[BITMAP_BUFFER_SIZE];

Buddy_allocator buddy_allocator;

/*
				visual representation of the buddy allocator's memory

								1048576                                         0

			 524288                                  524288                     1

	262144             262144              262144             262144            2

131072  131072     131072  131072      131072  131072     131072  131072        3

								   ...

1024                               ...                              1024        10


*/

int main(int argc, char **argv)
{
	buddy_allocator_init(&buddy_allocator,
						 bitmap_buffer,
						 BUDDY_ALLOCATOR_LEVELS,
						 memory,
						 BUDDY_ALLOCATOR_MIN_BUCKET_SIZE);

	buddy_allocator_malloc(&buddy_allocator, 65);
	bitmap_print(&buddy_allocator.map);
	buddy_allocator_malloc(&buddy_allocator, 65);
	bitmap_print(&buddy_allocator.map);
	buddy_allocator_malloc(&buddy_allocator, 65);
	bitmap_print(&buddy_allocator.map);
	buddy_allocator_malloc(&buddy_allocator, 65);
	bitmap_print(&buddy_allocator.map);
	buddy_allocator_malloc(&buddy_allocator, 65);
	bitmap_print(&buddy_allocator.map);
}