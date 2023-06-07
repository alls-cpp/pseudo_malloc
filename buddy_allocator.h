#pragma once

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "bitmap.h"

#define MAX_LEVELS 16

typedef struct
{
	Bitmap map;
	int num_levels;
	char *memory;		 // the memory area to be managed
	int min_bucket_size; // the minimum page of RAM that can be returned
} Buddy_allocator;

// initializes a buddy allocator
void buddy_allocator_init(Buddy_allocator *allocator,
						  uint8_t *buffer,
						  int num_levels,
						  char *memory,
						  int min_bucket_size);
// returns the memory address of a buddy
void *buddy_allocator_get_memory(Buddy_allocator *allocator, int level, int index);

// returns the level of the smallest bucket that can hold size bytes
int buddy_allocator_get_level(Buddy_allocator *allocator, int size);

// returns the index of the first available block at a given level
int buddy_allocator_first_available_block(const Bitmap *map, int level);

int buddy_allocator_get_buddy(Buddy_allocator *allocator, int level);

// allocates a block of memory of size size
void *buddy_allocator_malloc(Buddy_allocator *allocator, int size);

// frees a block of memory
void buddy_allocator_free(Buddy_allocator *allocator, int address);