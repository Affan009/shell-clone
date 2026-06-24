#ifndef JOBS_H
#define JOBS_H

typedef enum {
    RUNNING,
    STOPPED,
    DONE
} JobState;

typedef struct Job {
    int id;
    pid_t pid;
    JobState state;
    char* cmd[1024];

    struct Job* next;
    struct Job* prev;
} Job;

typedef struct JobList {
    Job* head;
    Job* tail;
    Job* current;
    Job* previous;
    int job_count;
} JobList;

JobList job_list = { NULL, NULL, NULL, NULL, 0 };

#endif
