#include "shell.h"
#include "jobs.h"

static JobList job_list = { NULL, NULL, 0 };

static int get_next_id() {
    if (job_list.tail == NULL) return 1;
    return job_list.tail->id + 1;
}

static Job* new_job(pid_t pid, char** args) {
    Job* job = malloc(sizeof(Job));
    if (!job) {
        perror("ash");
        return NULL;
    }

    job->id = get_next_id();
    job->pid = pid;
    job->state = RUNNING;
    job->next = NULL;
    job->prev = NULL;

    job->cmd[0] = '\0';
    for (int i = 0; args[i] != NULL; i++) {
        if (i > 0) strcat(job->cmd, " ");
        strcat(job->cmd, args[i]);
    }

    return job;
}

Job* add_job(pid_t pid, char** args) {
    Job* job = new_job(pid, args);
    if (!job) return NULL;

    job->prev = job_list.tail;

    if (job_list.tail == NULL) {
        job_list.head = job;
        job_list.tail = job;
    } else {
        job_list.tail->next = job;
        job_list.tail = job;
    }
    job_list.job_count++;

    return job;
}

Job* get_job_list_head() {
    return job_list.head;
}

Job* get_job_list_current() {
    return job_list.tail;
}

Job* get_job_list_previous() {
    return (job_list.tail == NULL) ? NULL : job_list.tail->prev;
}

bool is_background(char** args) {
    int i = 0;
    while (args[i] != NULL) i++;
    if (i > 0 && (strcmp(args[i - 1], "&") == 0)) {
        args[i - 1] = NULL;
        return true;
    }
    return false;
}
