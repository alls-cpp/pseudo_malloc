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

	assert(num_levels < MAX_LEVELS && "too many levels");

	// initialize the bitmap
	int num_bits = (1 << num_levels) - 1;
	bitmap_init(&allocator->map, buffer, num_bits);

	// mark all the memory as free
	bitmap_set_bit(&allocator->map, 0, 1);

	printf("buddy allocator initialized\n");
	printf("total memory: %d bytes\n", (1 << (num_levels - 1)) * min_bucket_size);
	printf("num_levels: %d\n", allocator->num_levels);
	printf("min_bucket_size: %d bytes\n", allocator->min_bucket_size);
}

// returns the memory address of a buddy
void *buddy_allocator_get_memory(Buddy_allocator *allocator,
								 int level,
								 int index)
{
	int start_level_index = (1 << level) - 1;
	int dimension_of_buddy = allocator->min_bucket_size * (1 << (allocator->num_levels - level - 1));
	int offset = (index - start_level_index) * dimension_of_buddy;

	printf("level: %d, index: %d, offset: %d\n", level, index, offset);

	return allocator->memory + offset;
}

// returns the level of the smallest bucket that can hold size bytes
int buddy_allocator_get_level(Buddy_allocator *allocator,
							  int size)
{
	int level = allocator->num_levels - 1;
	int size_of_level = allocator->min_bucket_size;
	while (size_of_level < size)
	{
		size_of_level *= 2;
		level--;
	}
	printf("%d bytes requested corresponds to level %d, that is %d bytes\n", size - sizeof(int), level, size_of_level);
	return level;
}

// returns the index of the first available block at a given level
int buddy_allocator_first_available_block(const Bitmap *map,
										  int level)
{
	int start = (1 << level) - 1;
	int end = (1 << (level + 1)) - 1;
	for (int i = start; i < end; i++)
	{
		if (bitmap_get_bit(map, i))
		{
			return i;
		}
	}
	return -1;
}

int buddy_allocator_get_buddy(Buddy_allocator *allocator,
							  int level)
{
	if (level < 0)
	{
		return -1;
	}

	int buddy_index = buddy_allocator_first_available_block(&allocator->map, level);

	if (buddy_index == -1)
	{
		// no block available at this level, try the previous one
		int parent_index = buddy_allocator_get_buddy(allocator, level - 1);

		// if there is no parent, there is no memory available
		if (parent_index == -1)
		{
			return -1;
		}

		// mark the parent as used
		bitmap_set_bit(&allocator->map, parent_index, 0);

		// mark the children as available
		// the left child will be used, so I don't set it to 1
		int left_child_index = parent_index * 2 + 1;
		int right_child_index = parent_index * 2 + 2;
		bitmap_set_bit(&allocator->map, right_child_index, 1);
		printf("split l:%d, left_idx: %d, right_idx: %d\r", level, left_child_index, right_child_index);

		buddy_index = left_child_index;
	}

	bitmap_set_bit(&allocator->map, buddy_index, 0);
	return buddy_index;
}

// allocates a block of memory of size size
void *buddy_allocator_malloc(Buddy_allocator *allocator,
							 int size)
{
	// TODO in the wrapper function, check the size, so it's a legal size

	// increase the size because I write the index of the block in the first 4 bytes
	size += sizeof(int);
	int level = buddy_allocator_get_level(allocator, size);

	// get the index of the buddy available at this level
	int buddy_index = buddy_allocator_get_buddy(allocator, level);

	// if there is no buddy available, return -1
	if (buddy_index == -1)
	{
		printf("no memory available\n");
		return NULL;
	}

	// get the address of the buddy
	int *address = (int *)buddy_allocator_get_memory(allocator, level, buddy_index);
	// write the index of the buddy in the first 4 bytes
	*address = buddy_index;

	// printf("allocated l:%d, idx: %d, address: %p\n", level, buddy_index, address);

	return address + 1;
}

// frees a block of memory
void buddy_allocator_free(Buddy_allocator *allocator,
						  int address)
{
}