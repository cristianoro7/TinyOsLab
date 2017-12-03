//
// Created by desperado on 17-12-1.
//

#ifndef JOBSCHEDULED_JOBSCHEDULER_H
#define JOBSCHEDULED_JOBSCHEDULER_H

#include <sys/time.h>
#include <assert.h>
#include <memory.h>
#include <time.h>
#include <malloc.h>

typedef enum state {
    WAIT,
    RUN,
    FINISH
} state;

typedef struct resources {
    long need_time;
    long memory_size;
} resources;

typedef struct job {
    char name[20];
    time_t submit_time;
    resources res;
    state stat;
    struct job *next;
    float priority;
    time_t finish_time;
} job;

static job *current = NULL;

static job *ready_queue = NULL;

static job *finish_queue = NULL;

static int job_counts = 0;

void insert_by_fcfs(job *j);

void insert_by_hrn(job *j);

char * get_submit_time(job *j);

char * get_finish_time(job *j);

float get_round_time(job *j);

float get_p_round_time(job *j); //带权周转时间

float get_avg_round_time();

float get_avg_p_round_time(); //平均带权周转时间

time_t get_current_time_stamp();

void init_finish_time_stamp(time_t start_time_stamp, job *finish_job);

void init_priority(job *j);

job *create_job(char *name, long need_time, long memory_size,
                int priority);

job *schedule_by_fcfs(job *finished_job);

job *schedule_by_hrn(job *finish_job);

void insert_into_finish_queue(job *finished);

int get_job_counts();

job *get_current_job();

job *get_finish_queue();

#endif //JOBSCHEDULED_JOBSCHEDULER_H
