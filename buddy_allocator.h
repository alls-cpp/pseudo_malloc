#pragma once
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "bitmap.h"

// TODO define MAX_LEVELS
#define MAX_LEVELS -1

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

// allocates a block of memory of size size
void buddy_allocator_malloc(Buddy_allocator *allocator, int size);

// frees a block of memory
void buddy_allocator_free(Buddy_allocator *allocator, int address);