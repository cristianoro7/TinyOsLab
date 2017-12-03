//
// Created by desperado on 17-11-18.
//

#ifndef OS_VIEW_H
#define OS_VIEW_H

#include "scheduler.h"

char *menu_list[] = {
        "1: 创建新进程.\n",
        "2: 展示正在运行的进程.\n",
        "3: 展示等待中的进程.\n",
        "4: 退出.\n",
        "请输入:\n"
};

int ask_create_task_num();

void create_and_insert_tasks(const int counts); //批量创建进程, 然后添加进等待队列

task *create_task(); //创建一个进程.

void schedule_and_run_task(task *t);

void display_ready_task();

void display_running_task();

void display_menu();

#endif //OS_VIEW_H
