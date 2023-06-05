#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "buddy_allocator.h"

// initializes a buddy allocator
void buddy_allocator_init(Buddy_allocator *allocator,
						  uint8_t *buffer,
						  int num_levels,
						  char *memory,
						  int min_bucket_size)
{
	// initialize the allocator
	allocator->num_levels = num_levels;
	allocator->memory = memory;
	allocator->min_bucket_size = min_bucket_size;

	assert(num_levels <= MAX_LEVELS && "too many levels");

	// initialize the bitmap
	int num_bits = (1 << num_levels) - 1;
	bitmap_init(&allocator->map, buffer, num_bits);

	// mark all the memory as free
	bitmap_set_bit(&allocator->map, 0, 1);

	printf("Buddy allocator initialized with %d levels, %d bytes of memory, and %d as the minimum bucket size\n",
		   num_levels, (1 << num_levels), min_bucket_size);
}

// allocates a block of memory of size size
void buddy_allocator_malloc(Buddy_allocator *allocator,
							int size)
{
	// TODO
}