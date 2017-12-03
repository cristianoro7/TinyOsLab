//
// Created by desperado on 17-12-3.
//

#include <stdio.h>
#include <stdlib.h>
#include "view.h"

void ask_for_heap_size() {
    printf("请输入堆的大小:\n");
    scanf("%ld", &h_size);
}

void ask_for_allocate_type() {
    printf("请输入分配算法: 0: 首次适应 1: 最佳适应\n");
    scanf("%d", &type);
}

node *allocate_by_type(long size) {
    if (!type) {
        return allocate_by_ff(size);
    } else {
        return allocate_by_bf(size);
    }
}

void display_free_list() {
    heap *h = get_free_list();
    while (h) {
        printf("| 起始地址: %ld | 大小: %ld | \n", h->header.base_address, h->header.heap_size);
        h = h->next;
    }
    printf("=========================================\n");
}

void is_allocate_success(long request_size, node *n) {
    if (n == NULL) {
        long rest_size = get_rest_memory();
        if (rest_size > request_size) {
            printf("分配内存失败. 要求分配的大小: %ld. 堆剩余的内存大小: %ld. 原因: 内存碎片化严重! 解决: 换用其他分配算法或者及时合并内存碎片!",
                   request_size, rest_size);
        } else {
            printf("分配内存失败. 要求分配的大小: %ld. 堆剩余的内存大小: %ld. 原因: 内存不足! 解决: 及时释放不需要的内存", request_size, rest_size);
        }
        exit(1);
    }
}

int main() {
    ask_for_heap_size();
    ask_for_allocate_type();

    init_heap(h_size, 1000);
    display_free_list();

    long job_1_size = 130;
    node *job_1 = allocate_by_type(job_1_size);
    is_allocate_success(job_1_size, job_1);
    display_free_list();

    long job_2_size = 60;
    node *job_2 = allocate_by_type(job_2_size);
    is_allocate_success(job_2_size, job_2);
    display_free_list();

    long job_3_size = 100;
    node *job_3 = allocate_by_type(job_3_size);
    is_allocate_success(job_3_size, job_3);
    display_free_list();

    free_memory(job_2);
    display_free_list();

    long job_4_size = 200;
    node *job_4 = allocate_by_type(job_4_size);
    is_allocate_success(job_4_size, job_4);
    display_free_list();

    free_memory(job_3);
    display_free_list();

    free_memory(job_1);
    display_free_list();

    long job_5_size = 140;
    node *job_5 = allocate_by_type(job_5_size);
    is_allocate_success(job_5_size, job_5);
    display_free_list();

    long job_6_size = 60;
    node *job_6 = allocate_by_type(job_6_size);
    is_allocate_success(job_6_size, job_6);
    display_free_list();

    long job_7_size = 50;
    node *job_7 = allocate_by_type(job_7_size);
    is_allocate_success(job_7_size, job_7);
    display_free_list();

    long job_8_size = 60;
    node *job_8 = allocate_by_type(job_8_size);
    is_allocate_success(job_8_size, job_8);
    display_free_list();

    return 0;
}