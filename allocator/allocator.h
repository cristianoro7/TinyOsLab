//
// Created by desperado on 17-12-3.
//

#ifndef ALLOCATOR_ALLOCATOR_H
#define ALLOCATOR_ALLOCATOR_H



typedef struct node {
    long size;
    long start_address;
} node;

typedef struct heap_header {
    long heap_size;
    long base_address;
} heap_header;

typedef struct heap {
    heap_header header;
    struct heap *next;
} heap;

static heap *g_heap = NULL;

void init_heap(long heap_size, long base_address);

node *allocate_by_ff(long size);

node *allocate_by_bf(long size);

long free_memory(node *m);

heap *get_free_list();

long get_rest_memory();

#endif //ALLOCATOR_ALLOCATOR_H
