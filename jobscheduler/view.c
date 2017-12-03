//
// Created by desperado on 17-12-1.
//
#include "view.h"
#include "jobscheduler.h"
#include <stdio.h>
#include <zconf.h>

int ask_for_job_counts() {
    int c = 0;
    printf("请出入要创建的作业数:\n");
    scanf("%d", &c);
    return c;
}

int ask_for_schedule_type() {
    int type = 0;
    printf("请选择调度的算法: 0: FCFS 1: HRN\n");
    scanf("%d", &type);
    return type;
}

void create_and_insert_job(int counts, int type) {
    int i = 0;
    for (i = 0; i < counts; i++) {
        printf("请输入作业%d的名字:\n", i + 1);
        char name[20];
        scanf("%s", name);
        long need_time = 0;
        printf("请输入作业%d需要的时间:\n", i + 1);
        scanf("%ld", &need_time);
        long memory_size = 0;
        printf("请输入作业%d需要的内存大小\n", i + 1);
        scanf("%ld", &memory_size);
        job *j = create_job(name, need_time, memory_size, -1);
        if (!type) {
            insert_by_fcfs(j);
        } else {
            init_priority(j);
            printf("%s的优先级为: %f", j->name, j->priority);
            insert_by_hrn(j);
        }
    }
}

void schedule_and_run_job(int type) {
    int i = 0;
    int counts = get_job_counts();
    for (i; i < counts; i++) {
        if (!type) {
            schedule_by_fcfs(get_current_job());

        } else {
            schedule_by_hrn(get_current_job());
        }
        printf("作业%s开始运行\n", get_current_job()->name);
        sleep(2);
        printf("作业%s完成任务\n", get_current_job()->name);
    }
    insert_into_finish_queue(get_current_job());
}

void display_job_information() {

    job *j = get_finish_queue();
    int i = 0;
    time_t pre_finish_time_stamp = -1;
    while (j) {
        char *submit_time = get_submit_time(j);
        char *finish_time;
        float round_time;
        float p_round_time;
        if (i == 0) {
            init_finish_time_stamp(j->submit_time, j);
        } else {
            init_finish_time_stamp(pre_finish_time_stamp, j);
        }
        finish_time = get_finish_time(j);
        round_time = get_round_time(j);
        p_round_time = get_p_round_time(j);

        printf("作业: %s | 开始运行时刻: %s | 完成时刻: %s | 周转时间: %0.10f | 带权周转时间: %0.10f |\n",
               j->name, submit_time, finish_time, round_time, p_round_time);
        pre_finish_time_stamp = j->finish_time;
        i++;
        j = j->next;
    }
    float avg_round_time = get_avg_round_time();
    float avg_p_round_time = get_avg_p_round_time();
    printf("平均周转时间: %0.10f | 平均带权周转时间: %0.10f", avg_round_time, avg_p_round_time);
}

int main() {
    int counts = ask_for_job_counts();
    int type = ask_for_schedule_type();
    create_and_insert_job(counts, type);
    schedule_and_run_job(type);
    display_job_information();
    return 0;
}
