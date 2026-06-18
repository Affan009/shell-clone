#include "shell.h"
#include "pipe.h"

char* trim(char* str) {
    char* end;
    while (isspace((unsigned char) *str)) str++;
    if (*str == '\0') return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_pipe(char* line) {
    int buffsize = SEG_BUFF_SIZE, position = 0;
    char** segments = malloc(buffsize * sizeof(char*));

    if (segments == NULL) {
        fprintf(stderr, "ash: allocation error\n");
        exit(EXIT_FAILURE);
    }

    char* segment = strtok(line, PIPE);
    while (segment != NULL) {
        segments[position++] = trim(segment);
        if (position >= buffsize) {
            buffsize += SEG_BUFF_SIZE;
            char** temp = realloc(segments, buffsize * sizeof(char*));
            if (temp == NULL) {
                free(segments);
                fprintf(stderr, "ash: allocation error\n");
                exit(EXIT_FAILURE);
            }
            segments = temp;
        }
        segment = strtok(NULL, PIPE);
    }

    segments[position] = NULL;
    return segments;
}

int pipe_execute(char** segments) {
    int i = 0;
    int fdin = STDIN_FILENO;

    while (segments[i] != NULL) {
        int fd[2];
        bool is_last = (segments[i + 1] == NULL);

        if (!is_last) pipe(fd);

        pid_t pid = fork();
        if (pid == 0) {
            // set stdin from previous pipe
            if (fdin != STDIN_FILENO) {
                dup2(fdin, STDIN_FILENO);
                close(fdin);
            }

            // set stdout to next pipe
            if (!is_last) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }

            // parse and handle redirection within this segment
            char** args = split_line(segments[i]);
            int redirected_fd = -1;
            int saved_fd = handle_redirection(args, &redirected_fd);
            if (redirected_fd != -1 && saved_fd == -1) {
                perror("ash");
                exit(EXIT_FAILURE);
            }

            sh_execute(args);
            // No need for saved_fd restoration in child

            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("ash");
            if (fdin != STDIN_FILENO) close(fdin);
            if (!is_last) {
                close(fd[0]);
                close(fd[1]);
            }
            break;
        }

        // parent closes used ends
        if (fdin != STDIN_FILENO) close(fdin);
        if (!is_last) {
            close(fd[1]);
            fdin = fd[0]; // next segment reads from this pipe
        }
        i++;
    }

    if (fdin != STDIN_FILENO) close(fdin);
    while (wait(NULL) > 0); // wait for all children to finish
    return 1;
}
