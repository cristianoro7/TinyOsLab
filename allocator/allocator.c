//
// Created by desperado on 17-12-3.
//
#include <malloc.h>
#include <assert.h>
#include "allocator.h"

void assert_not_null(void *p) {
    assert(p != NULL);
}

void init_heap(long heap_size, long base_address) {
    g_heap = malloc(sizeof(heap));
    assert_not_null(g_heap);
    heap_header header;
    header.base_address = base_address;
    header.heap_size = heap_size;
    g_heap->header = header;
    g_heap->next = NULL;
}

node *allocate_by_ff(long size) {
    heap *p = g_heap;
    while (p) {
        if (p->header.heap_size > size) {
            node *n = malloc(sizeof(node));
            assert_not_null(n);
            n->start_address = p->header.base_address;
            n->size = size;
            p->header.base_address += size;
            p->header.heap_size -= size;
            return n;
        }
        p = p->next;
    }
    return NULL;
}

long free_memory(node *m) {
    assert_not_null(m);
    heap *h = malloc(sizeof(heap));
    assert_not_null(h);
    h->header.heap_size = m->size;
    h->header.base_address = m->start_address;
    h->next = NULL;

    if (h->header.base_address < g_heap->header.base_address) {
        h->next = g_heap;
        g_heap = h;
    } else {
        heap *p = g_heap;
        heap *pre = p;
        while (p) {
            if (h->header.base_address < p->header.base_address) {
                pre->next = h;
                h->next = p;
                break;
            }
            pre = p;
            p = p->next;
            if (p == NULL) { //到达队尾
                pre->next = h;
                h->next = NULL;
            }
        }
    }
    return m->size;
}

node *allocate_by_bf(long size) {
    heap *p = g_heap;
    heap *bf = NULL;
    heap *temp = p;
    while (p) {
        if (p->header.heap_size >= size) {
            if (bf != NULL) {
                if (bf->header.heap_size >= p->header.heap_size) {
                    bf = p;
                }
            } else {
                bf = p;
            }
        }
        p = p->next;
    }
    if (bf != NULL && bf->header.heap_size >= size) {
        node *n = malloc(sizeof(node));
        assert_not_null(n);
        n->start_address = bf->header.base_address;
        n->size = size;
        bf->header.base_address += size;
        bf->header.heap_size -= size;
        return n;
    }
    return NULL;
}

heap *get_free_list() {
    return g_heap;
}

long get_rest_memory() {
    heap *p = g_heap;
    long size = 0;
    while (p) {
        size += p->header.heap_size;
        p = p->next;
    }
    return size;
}