#include "shell.h"

void sh_loop() {
    char* line;
    char** args;
    int status;

    init_completion();
    init_history();

    do {
        line = readline("$ ");
        char* line_copy = strdup(line);

        args = split_line(line);
        if (args[0] == NULL) { // Empty command
            free(line);
            free(line_copy);
            free(args);
            continue;
        }

        add_hist(line_copy);

        int redirected_fd = -1;
        int saved_fd = handle_redirection(args, &redirected_fd);
        if (redirected_fd != -1 && saved_fd == -1) { // Redirection error
            free(line);
            free(line_copy);
            free(args);
            continue;
        }

        status = sh_execute(args);

        if (saved_fd != -1) {
            dup2(saved_fd, redirected_fd);
            close(saved_fd);
        }

        free(line);
        free(line_copy);
        free(args);
    } while (status);
}
