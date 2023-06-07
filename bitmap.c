#include <stdlib.h>
#include <stdio.h>

#include "bitmap.h"

// red and green
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

// initializes a bitmap
void bitmap_init(Bitmap *bitmap,
				 uint8_t *buffer,
				 int num_bits)
{
	bitmap->buffer = buffer;
	bitmap->num_bits = num_bits;

	/*
	calculate the size of the buffer in bytes.

	division rounded up:
	result = (dividend + divisor - 1) / divisor
	*/
	bitmap->buffer_size = (num_bits + 7) / 8;
}

// returns the value of the bit bit_num
int bitmap_get_bit(const Bitmap *bitmap,
				   int bit_num)
{
	int byte_num = bit_num / 8;
	int bit_in_byte = bit_num % 8;

	return (bitmap->buffer[byte_num] >> bit_in_byte) & 1;
}

// sets the bit bit_num to status
void bitmap_set_bit(Bitmap *bitmap,
					int bit_num,
					int status)
{
	if (bit_num < 0 || bit_num >= bitmap->num_bits)
	{
		printf("bit_num out of range\n");
		exit(1);
	}

	int byte_num = bit_num / 8;
	int bit_in_byte = bit_num % 8;

	if (status)
	{
		bitmap->buffer[byte_num] |= (1 << bit_in_byte);
	}
	else
	{
		bitmap->buffer[byte_num] &= ~(1 << bit_in_byte);
	}
}

// prints the bitmap
void bitmap_print(const Bitmap *bitmap)
{
	int num_levels = 0;
	int num_bits = bitmap->num_bits;

	while (num_bits > 0)
	{
		num_bits /= 2;
		num_levels++;
	}

	int level = 0;
	int num_bits_in_level = 1;
	int bit_num = 0;
	int padding = (1 << num_levels) - 1;

	printf("\n");

	while (level < num_levels)
	{
		// print left padding
		for (int i = 0; i < padding; i++)
		{
			printf(" ");
		}

		padding = padding * 2 + 1;

		// print bits
		for (int i = 0; i < num_bits_in_level; i++)
		{
			if (bitmap_get_bit(bitmap, bit_num))
			{
				printf(COLOR_GREEN "1" COLOR_RESET);
			}
			else
			{
				printf(COLOR_RED "0" COLOR_RESET);
			}
			bit_num++;

			// print right padding
			for (int i = 0; i < padding; i++)
			{
				printf(" ");
			}
		}

		printf("\n");

		num_bits_in_level *= 2;
		padding /= 4;
		level++;
	}

	printf("\n");
}