#include "shell.h"

void sh_loop() {
    char* line;
    char** args;
    int status;

    do {
        printf("$ ");

        line = read_line();
        args = split_line(line);
        status = sh_execute(args);

        free(line);
        free(args);
    } while (status);
}
