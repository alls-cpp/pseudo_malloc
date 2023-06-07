#include <stdio.h>

#include "pseudo_allocator.h"

int main(int argc, char **argv)
{
	pseudo_allocator_init();
	void *ptr = pseudo_malloc(100);
}