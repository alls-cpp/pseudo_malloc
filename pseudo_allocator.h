#pragma once

#include "buddy_allocator.h"

void pseudo_allocator_init();

void *pseudo_malloc(int size);

void pseudo_free(void *address);