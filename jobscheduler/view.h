//
// Created by desperado on 17-12-1.
//

#ifndef JOBSCHEDULED_VIEW_H
#define JOBSCHEDULED_VIEW_H

#define FCFS 0;
#define HRN 1;

int ask_for_job_counts();

int ask_for_schedule_type();

void create_and_insert_job(int counts, int type);

void display_job_information();



#endif //JOBSCHEDULED_VIEW_H
