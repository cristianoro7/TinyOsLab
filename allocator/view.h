//
// Created by desperado on 17-12-3.
//

#ifndef ALLOCATOR_VIEW_H
#define ALLOCATOR_VIEW_H

#include "allocator.h"

static long h_size;


static int type;

void ask_for_heap_size();

void ask_for_allocate_type();

void display_free_list();

node *allocate_by_type(long size);

#endif //ALLOCATOR_VIEW_H
