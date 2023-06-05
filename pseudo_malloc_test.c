#include <stdio.h>

#include "buddy_allocator.h"

#define MEMORY_SIZE (1 << 20) // 1 MB

int main()
{
    printf("memory size: %d\n", MEMORY_SIZE);
}