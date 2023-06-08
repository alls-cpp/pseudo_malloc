#include <stdio.h>
#include <math.h>

#include "buddy_allocator.h"

/*
				visual representation of the buddy allocator's memory

								1048576                                         0

			 524288                                  524288                     1

	262144             262144              262144             262144            2

131072  131072     131072  131072      131072  131072     131072  131072        3

								   ...

1024                               ...                              1024        10
*/

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

	// initialize the bitmap
	int num_bits = (1 << num_levels) - 1;
	bitmap_init(&allocator->map, buffer, num_bits);

	// mark all the memory as free
	bitmap_set_bit(&allocator->map, 0, 1);

	printf("buddy allocator initialized\n");
	printf("\ttotal memory:    %d bytes\n", (1 << (num_levels - 1)) * min_bucket_size);
	printf("\tnum_levels:      %d\n", allocator->num_levels);
	printf("\tmin_bucket_size: %d bytes\n\n", allocator->min_bucket_size);
}

// returns the memory address of a buddy
void *buddy_allocator_get_memory(Buddy_allocator *allocator,
								 int level,
								 int index)
{
	int start_level_index = (1 << level) - 1;
	int dimension_of_buddy = allocator->min_bucket_size * (1 << (allocator->num_levels - level - 1));
	int offset = (index - start_level_index) * dimension_of_buddy;
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
	printf("%ld bytes requested corresponds to level %d, that is %d bytes\n", size - sizeof(int), level, size_of_level);
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

		printf("\tsplitting %d into %d and %d\n", parent_index, left_child_index, right_child_index);

		buddy_index = left_child_index;
	}

	bitmap_set_bit(&allocator->map, buddy_index, 0);
	return buddy_index;
}

void buddy_allocator_release_buddy(Buddy_allocator *allocator,
								   int index)
{
	int brother_index = index & 1 ? index + 1 : index - 1;
	int parent_index = (index - 1) / 2;

	// if the brother is free, merge the two blocks
	if (bitmap_get_bit(&allocator->map, brother_index))
	{
		// mark the parent as free
		bitmap_set_bit(&allocator->map, parent_index, 1);

		bitmap_set_bit(&allocator->map, index, 0);
		bitmap_set_bit(&allocator->map, brother_index, 0);

		printf("\tmerging %d and %d into %d\n", index, brother_index, parent_index);

		// repeat recursively
		buddy_allocator_release_buddy(allocator, parent_index);
	}
	else
	{
		// mark the block as free
		bitmap_set_bit(&allocator->map, index, 1);
	}
}

// allocates a block of memory of size size
void *
buddy_allocator_malloc(Buddy_allocator *allocator,
					   int size)
{
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

	printf("allocated level %d, idx %d, address %p\n\n", level, buddy_index, address);

	// print the bitmap only if there are less than 7 levels, otherwise it's too big
	if (allocator->num_levels < 7)
		bitmap_print(&allocator->map, buddy_index, 0);

	return address + 1;
}

// frees a block of memory
void buddy_allocator_free(Buddy_allocator *allocator,
						  void *address)
{
	// get the index of the buddy
	int buddy_index = *((int *)address - 1);

	if (buddy_index < 0 || buddy_index >= allocator->map.num_bits)
	{
		printf("trying to free a pointer that was not allocated by this allocator\n");
		return;
	}

	printf("freed level %d, idx %d, address %p\n", allocator->num_levels - 1, buddy_index, address);
	buddy_allocator_release_buddy(allocator, buddy_index);

	// print the bitmap only if there are less than 7 levels, otherwise it's too big
	if (allocator->num_levels < 7)
		bitmap_print(&allocator->map, buddy_index, 1);
}