#include "shell.h"
#include "builtins.h"

const Builtin builtins[] = {
    {"exit", sh_exit},
    {"echo", sh_echo},
    {"type", sh_type},
    {"pwd", sh_pwd},
    {"cd", sh_cd},
    {"help", sh_help},
    {"history", sh_history},
    {"jobs", sh_jobs}
};

// Helper function to get the number of built-in commands
int num_builtins() {
    return sizeof(builtins) / sizeof(Builtin);
}

int sh_exit(char** args) {
    (void)args; // Unused parameter
    return 0;
}

int sh_echo(char** args) {
    for (size_t i = 1; args[i] != NULL; i++) {
        printf("%s", args[i]);
        if (args[i + 1] != NULL) printf(" ");
    }
    printf("\n");
    return 1;
}

int sh_type(char** args) {
    if (args[1] == NULL) {
        return 1;
    } else if (is_builtin(args[1])) {
        printf("%s is a shell builtin\n", args[1]);
    } else {
        // PATH lookup for program
        const char* path = getenv("PATH");
        if (path == NULL) {
            fprintf(stderr, "ash: PATH not set\n");
            return 1;
        }

        char* path_copy = strdup(path);
        for (char* dir = strtok(path_copy, ":"); dir != NULL; dir = strtok(NULL, ":")) {
            char full_path[strlen(dir) + strlen(args[1]) + 2];
            sprintf(full_path, "%s/%s", dir, args[1]);

            if (access(full_path, X_OK) == 0) {
                printf("%s is %s\n", args[1], full_path);
                free(path_copy);
                return 1;
            }
        }
        printf("%s: not found\n", args[1]);
        free(path_copy);
    }
    return 1;
}

int sh_pwd(char** args) {
    (void)args; // Unused parameter
    char* path = getcwd(NULL, 0);

    if (path == NULL) {
        perror("ash");
    } else {
        printf("%s\n", path);
        free(path);
    }

    return 1;
}

int sh_cd(char** args) {
    const char* path = NULL;
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "ash: HOME not set\n");
            return 1;
        }
    } else {
        path = args[1];
    }

    if (chdir(path) != 0) {
        perror("ash");
    }
    return 1;
}

int sh_help(char** args) {
    (void)args; // Unused parameter
    printf("ash: a C-shell (nice)\n");
    printf("Built-in commands:\n");

    for (int i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtins[i].name);
    }
    return 1;
}

int sh_history(char** args) {
    if (args[1] != NULL && strcmp(args[1], "-r") == 0) {
        if (args[2]) read_hist(args[2]);
        return 1;
    }

    if (args[1] != NULL && strcmp(args[1], "-w") == 0) {
        if (args[2]) write_hist(args[2]);
        return 1;
    }

    if (args[1] != NULL && strcmp(args[1], "-a") == 0) {
        if (args[2]) append_hist(args[2]);
        return 1;
    }

    int start = 0;
    if (args[1] != NULL) {
        int cap = atoi(args[1]);
        start = get_hist_count() - cap;
        if (start < 0) start = 0;
    }

    for (int i = start; i < get_hist_count(); i++) {
        printf("%d: %s\n", i + 1, get_hist_entry(i));
    }
    return 1;
}

int sh_jobs(char** args) {
    (void)args;

    // first check for newly finished processes
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        Job* job = find_job_by_pid(pid);
        if (job) job->state = DONE;
    }

    for (Job* job = get_job_list_head(); job; job = job->next) {
        char marker = (job == get_job_list_current()) ? '+' : (job == get_job_list_previous()) ? '-' : ' ';
        char* state_str = (job->state == RUNNING) ? "Running" : (job->state == STOPPED) ? "Stopped" : "Done";
        char* suffix = (job->state == RUNNING) ? " &" : "";
        printf("[%d]%c  %-22s%s%s\n", job->id, marker, state_str, job->cmd, suffix);
    }

    // remove Done jobs after printing
    Job* curr = get_job_list_head();
    while (curr != NULL) {
        Job* next = curr->next;
        if (curr->state == DONE)
            remove_job(curr);
        curr = next;
    }

    return 1;
}

bool is_builtin(const char* command) {
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(command, builtins[i].name) == 0) {
            return true;
        }
    }
    return false;
}
