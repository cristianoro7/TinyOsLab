//
// Created by desperado on 17-11-18.
//
#include <stdio.h>
#include <sys/time.h>
#include "view.h"
#include <signal.h>
#include <zconf.h>


static void scanf_name(char *return_msg) {
    scanf("%s", return_msg);
}

static void scanf_time(long *time) {
    scanf("%ld", time);
}

static void easy_printf(char *msg) {
    if (msg != NULL) {
        printf("%s", msg);
    }
}

long get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


int ask_create_task_num() {
    int count = 0;
    easy_printf("请输入你要创建的进程数:\n");
    while (1) {
        scanf("%d", &count);
        if (count > 0) {
            break;
        } else {
            easy_printf("请输入大于0的数字:\n");
        }
    }
    return count;
}

void create_and_insert_tasks(const int counts) {
    int i;
    for (i = 0; i < counts; i++) {
        printf("请输入第%d个进程的名字:", i + 1);
        char name[128];
        scanf_name(name);
        printf("请输入第%d个进程需要的时间片:", i + 1);
        long need_time = 0;
        scanf_time(&need_time);
        task *t = create(name, CREATED, get_current_time(), need_time, 0, NULL);
        insert_into_ready_queue(t);
    }
}

task *create_task() {
    easy_printf("请输入进程的名字:");
    char name[128];
    scanf_name(name);
    easy_printf("请输入进程需要的时间片:");
    long need_time = 0;
    scanf_time(&need_time);
    return create(name, CREATED, get_current_time(), need_time, 0, NULL);
}


void handler(int signo) {
    task *death_task = get_current();
    printf("进程 %s 运行完毕, 运行的时间为: %ld\n", death_task->name, death_task->used_time);
    destroy_task();
    schedule_next(NULL);

    task *t = get_current();
    if (t) {
        printf("新运行的进程为:%s, 运行所需的时间为:%ld\n", t->name, t->need_time);
    } else {
        easy_printf("所有进程已经运行完毕\n");
        stopTimer(); //关闭定时器
    }
}

void schedule_and_run_task(task *t) {
    schedule_next(t);
    if (!is_timer_running()) {
        startTimer(2, handler); //如果没有开启定时器的话, 开启定时器
    }
}

void display_ready_task() {
    task *t = get_ready();
    if (!t) {
        printf("此刻没有进程在等待队列, 请添加进程\n");
    } else {
        printf("| name | need_time | used_time | rest_time |\n");
        while (t) {
            printf("   %s \t\t  %ld  \t\t  %ld  \t\t  %ld  \n", t->name, t->need_time, t->used_time,
                   t->need_time - t->used_time);
            t = t->next;
        }
    }
}

void display_running_task() {
    task *t = get_current();
    if (t) {
        printf("| name | need_time | used_time | rest_time |\n");
        printf("   %s \t\t  %ld  \t\t  %ld  \t\t  %ld  \n", t->name, t->need_time, t->used_time,
               get_task_rest_time(t));
    } else {
        printf("此刻没有进程, 请添加进程!\n");
    }
}

void display_menu() {
    int is_running = 1;
    int cmd = -1;
    while (is_running) {
        for (int i = 0; i < 5; ++i) {
            easy_printf(menu_list[i]);
        }
        scanf("%d", &cmd);
        switch (cmd) {
            case 1: {
                task *new = create_task();
                insert_into_ready_queue(new);
                schedule_and_run_task(NULL);
                break;
            }
            case 2:
                display_running_task();
                break;
            case 3:
                display_ready_task();
                break;
            case 4:
                is_running = 0;
                break;
            default:
                break;
        }
    }
}

int main() {
    int counts = ask_create_task_num();
    create_and_insert_tasks(counts);
    schedule_and_run_task(NULL);
    startTimer(2, handler);
    display_menu();
    return 0;
}






