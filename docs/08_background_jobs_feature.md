# Background Jobs

Shell runs a terminal command in the foreground. A background job is a command that is run without blocking the shell.

### Goals
- Implement background job execution with unblocking behavior.
- Implement the jobs builtin and the job table.
- Implement handler for SIGCHLD signal to reap finished background jobs.

## Design
- A command suffixed with `&` should run be executed in the background, without the shell waiting for it in the foreground.
- The jobs builtin should show the jobs currently being tracked in the background.
- These jobs should be reaped after they finish execution, when they fire the SIGCHLD signal.

## Implementation Details

### Data Structures
- An `enum` to represent job state, aliased as `JobState`:
    ``` C
    typedef enum {
        RUNNING,
        STOPPED,
        DONE
    } JobState;
    ```

- A job node containing `pid` and the `command`, alongside pointers to the previous and next nodes:
    ``` C
    typedef struct Job {
        int id;
        pid_t pid;
        JobState state;
        char cmd[1024];

        struct Job* next;
        struct Job* prev;
    } Job;
    ```

- A `job_list` which is doubly linked and also holds the `job_count`:
    ``` C
    typedef struct JobList {
        Job* head;
        Job* tail;

        int job_count;
    } JobList;

    static JobList job_list = { NULL, NULL, 0 }
    ```

### Functions
| Function | Parameters | Return Value | Description |
|----------|------------|--------------|-------------|
|`new_job(pid, args)`|`pid_t` — pid of the child process and `char**` — arguments provided to the launch function|`Job*` - Job pointer|Return a newly created job, populated with `id` (recycled), provided `pid`, `RUNNING` state, and the command string from `args`|
|`add_job(pid, args)`|`pid_t` — pid of the child process and `char**` — arguments provided to the launch function|`Job*` — Job pointer|Return a job added to the `job_list` and created by `new_job(pid, args)`|
|`get_job_list_head()`|None (Void)|`Job*` — Job pointer|Return the head of the `job_list`|
|`get_job_list_current()`|None (Void)|`Job*` — Job pointer|Return the pointer to the current job node in the `job_list` (which is the `tail`)|
|`get_job_list_previous()`|None (Void)|`Job*` — Job pointer|Return the pointer to the previous job of the current job node in the `job_list` (which is `tail->prev`)|
|`get_next_id()`|None (Void)|`int` — Integer id|Return the integer id next to the highest job id in the `job_list`|
|`is_background(args)`|`char**` — arguments|`bool` — a boolean|Return `true` if the command is supposed to run in the background|

