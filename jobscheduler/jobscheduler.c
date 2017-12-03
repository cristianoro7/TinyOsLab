//
// Created by desperado on 17-12-1.
//
#include <stddef.h>
#include "jobscheduler.h"

int get_job_counts() {
    return job_counts;
}

job *get_current_job() {
    return current;
}

void assert_not_null(void *p) {
    assert(p != NULL);
}

void insert_by_fcfs(job *j) {
    assert_not_null(j);
    j->stat = WAIT;
    if (ready_queue == NULL) {
        ready_queue = j;
    } else {
        job *p = ready_queue;
        while (p) {
            if (p->next == NULL) {
                p->next = j;
                j->next = NULL;
                break;
            }
            p = p->next;
        }
    }
    job_counts++;
}

char *get_time_with_string(const time_t *t) {
    struct tm tm;
    gmtime_r(t, &tm);
    tm.tm_hour += 8;
    char *s = malloc(sizeof(char) * 100);
    strftime(s, 100, "%Y:%m:%d:%H:%M:%S", &tm);
    return s;
}

char *get_submit_time(job *j) {
    assert_not_null(j);
    return get_time_with_string(&j->submit_time);
}

char *get_finish_time(job *j) {
    assert_not_null(j);
    return get_time_with_string(&j->finish_time);
}

float get_round_time(job *j) {
    assert_not_null(j);
    return j->finish_time - j->submit_time;
}

float get_p_round_time(job *j) {
    assert_not_null(j);
    return get_round_time(j) / j->res.need_time;
}

float get_avg_round_time() {
    float total_round_time = 0;
    job *p = finish_queue;

    while (p) {
        total_round_time += get_round_time(p);
        p = p->next;
    }
    return total_round_time / job_counts;
}

float get_avg_p_round_time() {
    float t = 0;
    job *p = finish_queue;

    while (p) {
        t += get_p_round_time(p);
        p = p->next;
    }
    return t / job_counts;
}

void init_priority(job *j) {
    assert_not_null(&j);
    float priority = get_current_time_stamp() / j->res.need_time; //初始的优先级直接设置为需要的时间, 你.
    j->priority = priority;
}

void insert_by_hrn(job *j) {
    assert_not_null(j);
    j->stat = WAIT;
    if (ready_queue == NULL || ready_queue->priority < j->priority) {
        j->next = ready_queue;
        ready_queue = j;
    } else {
        job *p = ready_queue;
        job *pre = p;
        while (p) {
            if (p->priority < j->priority) {
                pre->next = j;
                j->next = p;
                break;
            }
            pre = p;
            p = p->next;
            if (p == NULL) { //到达队尾
                pre->next = j;
                j->next = NULL;
            }
        }
    }
    job_counts++;
}

time_t get_current_time_stamp() {
    time_t t;
    time(&t);
    return t;
}

void init_finish_time_stamp(time_t start_time_stamp, job *finish_job) {
    assert_not_null(finish_job);
    time_t finish_time_stamp = start_time_stamp + finish_job->res.need_time;
    finish_job->finish_time = finish_time_stamp;
}

job *create_job(char *name, long need_time, long memory_size,
                int priority) {
    assert_not_null((void *) name);

    job *j = malloc(sizeof(job));
    assert_not_null(j);
    strcpy(j->name, name);
    resources res;
    res.need_time = need_time;
    res.memory_size = memory_size;
    j->res = res;
    j->priority = priority;
    j->submit_time = get_current_time_stamp();
    j->next = NULL;
    j->finish_time = -1;
    return j;
}

job *take_first_from_ready_queue() {
    job *first = ready_queue;
    ready_queue = ready_queue->next;
    return first;
}

void insert_into_finish_queue(job *finished) {
    if (finished != NULL) {
        finished->stat = FINISH;
    }
    if (finish_queue == NULL) {
        finish_queue = finished;
    } else {
        job *p = finish_queue;
        while (p) {
            if (p->next == NULL) {
                p->next = finished;
                finished->next = NULL;
                break;
            }
            p = p->next;
        }
    }
}

job *schedule_by_fcfs(job *finish_job) {
    insert_into_finish_queue(finish_job);
    current = take_first_from_ready_queue();
    current->stat = RUN;
    return current;
}

void calculate_priority(job *j) {
    assert_not_null(j);
    time_t waiting_time = get_current_time_stamp() - j->submit_time;
    j->priority = (waiting_time + j->res.need_time) / j->res.need_time;
}

void sort_ready_queue_by_priority() {
    job *p = ready_queue;
    while (p) {
        calculate_priority(p);
        p = p->next;
    }

    job *j = ready_queue;
    job *pre;
    ready_queue = NULL;
    //开始重新插入等待队列
    while (j) {
        job_counts--;
        pre = j;
        j = j->next;
        insert_by_hrn(pre);
    }
}

job *schedule_by_hrn(job *finish_job) {
    if (finish_job == NULL) {
        current = take_first_from_ready_queue(); //NULL的话, 直接拿等待队列的第一个
    } else {
        insert_into_finish_queue(finish_job);
        sort_ready_queue_by_priority(); //重新计算优先级
        current = take_first_from_ready_queue(); //然后从等待队列拿第一个作业进行调度
    }
    current->stat = RUN;
    return current;
}

job *get_finish_queue() {
    return finish_queue;
}

