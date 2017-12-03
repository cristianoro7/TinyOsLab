//
// Created by desperado on 17-11-14.
//

#ifndef OS_SCHEDULER_H
#define OS_SCHEDULER_H


#include <stddef.h>
#include <pthread.h>
#include <signal.h>
#include <zconf.h>
#include <sys/time.h>

typedef enum state {
    CREATED, //创建状态
    WAITING, //就绪状态
    RUNNING, //运行状态
    FINISH //销毁状态
} state;

typedef struct task {
    char name[20];
    state stat;
    long reach_time; //进程到达的时间
    long need_time; //需要用的CPU时间
    long used_time; //已用CPU时间
    struct task *next; //下个进程指针
} task;

static task *ready = NULL; //就绪队列

static task *current = NULL; //当前正在运行的进程

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static long task_counts = 0;

static int is_start_timer = 0; //定时器开关

task *create(const char *name, const enum state st, const long reach_time,
             const long need_time, const long used_time, task *next);

void schedule_next(task *t);

void destroy_task();

void insert_into_ready_queue(task *new_task);

task *get_ready();

task *get_current();

long get_task_rest_time(task *t);

task *remove_from_ready_queue();

void startTimer(int second, void handler(int signo));

void stopTimer();

int is_timer_running();

#endif //OS_SCHEDULER_H
