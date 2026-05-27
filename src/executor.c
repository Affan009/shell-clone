#include "shell.h"
#include "executor.h"

int sh_launch(char** args) {
    pid_t pid;

    pid = fork();
    if (pid == 0) { // Child process
        if (execvp(args[0], args) == -1) {
            if (errno == ENOENT) { // No such file or directory
                fprintf(stderr, "ash: command not found: %s\n", args[0]);
            } else {
                perror("ash");
            }
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) { // Error forking
        perror("ash");
    } else { // Parent process
        pid_t wpid;
        int status;
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int sh_execute(char** args) {
    if (args[0] == NULL) { // Empty command
        return 1;
    }

    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            return builtins[i].func(args);
        }
    }

    return sh_launch(args);
}

