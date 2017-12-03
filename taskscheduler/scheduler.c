//
// Created by desperado on 17-11-14.
//
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "scheduler.h"

task *create(const char name[], const state st, const long reach_time,
             const long need_time, const long used_time, task *next) {
    task *t = (task *) malloc(sizeof(task));
    if (NULL == t) {
        exit(1);
    }
    strcpy(t->name, name);
    t->stat = st;
    t->reach_time = reach_time;
    t->need_time = need_time;
    t->used_time = used_time;
    t->next = next;
    return t;
}

void destroy_task() {
    pthread_mutex_lock(&mutex);
    if (current != NULL) {
        free(current);
    }
    task_counts--;
    pthread_mutex_unlock(&mutex);
}

long get_task_rest_time(task *t) {
    assert(t != NULL);
    return t->need_time - t->used_time;
}

void insert_into_ready_queue(task *new_task) {
    if (NULL == new_task) {
        exit(1);
    }
    new_task->stat = WAITING; //更改状态
    if (ready == NULL || get_task_rest_time(new_task) < get_task_rest_time(ready)) {
        new_task->next = ready;
        ready = new_task;
    } else {
        task *second = ready;
        task *first = ready;
        while (first != NULL) { //second == NULL表示到达末尾, 直接插入到末尾
            if (second == NULL || get_task_rest_time(second) > get_task_rest_time(new_task)) {
                new_task->next = second;
                first->next = new_task;
                break;
            }
            first = second;
            second = second->next;
        }
    }
    task_counts++;
}

//void remove_from_ready_queue(task *t) {
//    assert(t != NULL);
//    task *p = ready;
//    task *pre = ready;
//    while (p) {
//        if (strcmp(p->name, t->name) == 0) {
//            pre->next = p->next;
//            p->next = NULL;
//        }
//        pre = p;
//        p = p->next;
//    }
//}

task *remove_from_ready_queue() {
    if (NULL == ready) {
        return NULL;
    }
    task *remove = ready;
    ready = ready->next;
    return remove;
}

task *get_ready() {
    return ready;
}

task *get_current() {
    return current;
}

void schedule_next(task *t) {
    if (NULL == t) {
        current = remove_from_ready_queue(); //如果t为空的话, 直接从等待队列中拿队头的进程进行调度
    } else { //不为空的话, 跟正在运行的进程比较一下剩余的时间片, 以此来决定是否要抢占调度.
        if (current != NULL && get_task_rest_time(t) < get_task_rest_time(current)) {
            //进行抢占调度
            //步骤:
            //1: 将当前运行的进程插入到等待队列中
            //2: 将正在运行的进程设置为t
            task *task_to_insert = current;
            insert_into_ready_queue(task_to_insert);
            current = t;
        } else {
            insert_into_ready_queue(t);
        }
    }
}

void *call_back(void *arg) {
    struct sigaction act;
    union sigval tsval;
    act.sa_handler = arg;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(50, &act, NULL);
    unsigned int sleep_time = 2;

    is_start_timer = 1;
    while (is_start_timer) {
        sleep(sleep_time);
        pthread_mutex_lock(&mutex);
        if (current != NULL) {
            if (get_task_rest_time(current) > 0) {
                current->used_time++; //递增已用时间片
            } else {
                sigqueue(getpid(), 50, tsval); //发送一个信号, 让主线程进行线程调度
            }
        }
        pthread_mutex_unlock(&mutex);

    }
}

void startTimer(int second, void handler(int signo)) {
    pthread_t tid;
    pthread_create(&tid, NULL, call_back, handler);
}

void stopTimer() {
    pthread_mutex_lock(&mutex);
    is_start_timer = 0;
    pthread_mutex_unlock(&mutex);
}

int is_timer_running() {
    return is_start_timer;
}


