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
    char cmd[1024];

    struct Job* next;
    struct Job* prev;
} Job;

typedef struct JobList {
    Job* head;
    Job* tail;
    
    int job_count;
} JobList;

Job* add_job(pid_t pid, char** args);

Job* get_job_list_head();
Job* get_job_list_current();
Job* get_job_list_previous();

bool is_background(char** args);

#endif
