#pragma once

#include <stdint.h>

typedef struct
{
	uint8_t *buffer; // externally allocated buffer
	int num_bits;
	int buffer_size;
} Bitmap;

// initializes a bitmap
void bitmap_init(Bitmap *bitmap, uint8_t *buffer, int num_bits);

// returns the value of the bit bit_num
int bitmap_get_bit(const Bitmap *bitmap, int bit_num);

// sets the bit bit_num to status
void bitmap_set_bit(Bitmap *bitmap, int bit_num, int status);

// prints the bitmap
void bitmap_print(const Bitmap *bitmap);