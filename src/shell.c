#include "shell.h"

void sh_loop() {
    char* line;
    char** args;
    int status;

    do {
        printf("$ ");

        line = read_line();
        args = split_line(line);
        if (args[0] == NULL) { // Empty command
            free(line);
            free(args);
            continue;
        }

        int redirected_fd = -1;
        int saved_fd = handle_redirection(args, &redirected_fd);
        if (redirected_fd != -1 && saved_fd == -1) { // Redirection error
            free(line);
            free(args);
            continue;
        }

        status = sh_execute(args);

        if (saved_fd != -1) {
            dup2(saved_fd, redirected_fd);
            close(saved_fd);
        }

        free(line);
        free(args);
    } while (status);
}
