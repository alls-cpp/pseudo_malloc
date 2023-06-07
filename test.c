#include <stdio.h>
#include <assert.h>

#include "pseudo_allocator.h"

int main(int argc, char **argv)
{
	// initialize the buddy allocator
	pseudo_allocator_init();

	printf("Test Case 1: buddy_allocator_malloc and buddy_allocator_free\n");
	int *ptr1 = (int *)pseudo_malloc(sizeof(int));
	assert(ptr1 != NULL);
	printf("Allocated memory: %p\n\n", ptr1);

	int *ptr2 = (int *)pseudo_malloc(2 * sizeof(int));
	assert(ptr2 != NULL);
	printf("Allocated memory: %p\n\n", ptr2);

	pseudo_free(ptr1);
	printf("Freed memory: %p\n\n", ptr1);

	pseudo_free(ptr2);
	printf("Freed memory: %p\n\n", ptr2);

	pseudo_bitmap_print();
}