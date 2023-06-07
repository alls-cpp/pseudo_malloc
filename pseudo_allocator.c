#include <sys/mman.h>
#include <stdio.h>

#include "pseudo_allocator.h"

#define PAGE_SIZE 4096

#define BUDDY_ALLOCATOR_MEMORY_SIZE (1 << 20) // 1 MB
#define BUDDY_ALLOCATOR_LEVELS 5
#define BUDDY_ALLOCATOR_MIN_BUCKET_SIZE (BUDDY_ALLOCATOR_MEMORY_SIZE >> (BUDDY_ALLOCATOR_LEVELS - 1))
#define BITMAP_BUFFER_SIZE (1 << (BUDDY_ALLOCATOR_LEVELS + 1))

char memory[BUDDY_ALLOCATOR_MEMORY_SIZE];
uint8_t bitmap_buffer[BITMAP_BUFFER_SIZE];

Buddy_allocator buddy_allocator;

void pseudo_allocator_init()
{
    buddy_allocator_init(&buddy_allocator,
                         bitmap_buffer,
                         BUDDY_ALLOCATOR_LEVELS,
                         memory,
                         BUDDY_ALLOCATOR_MIN_BUCKET_SIZE);
}

void *pseudo_malloc(int size)
{
    if (size <= 0)
    {
        printf("invalid size\n");
        return NULL;
    }

    if (size < PAGE_SIZE / 4)
    {
        return buddy_allocator_malloc(&buddy_allocator, size);
    }
    else
    {
        void *address = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (address == MAP_FAILED)
        {
            perror("mmap failed");
            return NULL;
        }
        return address;
    }
}

void pseudo_free(void *address)
{
    if (address <= 0)
    {
        printf("invalid address\n");
        return;
    }
    if (address >= (void *)memory && address < (void *)memory + BUDDY_ALLOCATOR_MEMORY_SIZE)
    {
        buddy_allocator_free(&buddy_allocator, address);
    }
    else
    {
        int result = munmap(address, 0);
        if (result == -1)
        {
            perror("munmap failed");
        }
    }
}

void pseudo_bitmap_print()
{
    bitmap_print(&buddy_allocator.map, -1, -1);
}