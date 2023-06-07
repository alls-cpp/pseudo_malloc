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

int main(int argc, char **argv)
{
	buddy_allocator_init(&buddy_allocator,
						 bitmap_buffer,
						 BUDDY_ALLOCATOR_LEVELS,
						 memory,
						 BUDDY_ALLOCATOR_MIN_BUCKET_SIZE);

	void *address = buddy_allocator_malloc(&buddy_allocator, 1024);
	address = buddy_allocator_malloc(&buddy_allocator, 1024);
	address = buddy_allocator_malloc(&buddy_allocator, 1024);
	buddy_allocator_free(&buddy_allocator, address);
}