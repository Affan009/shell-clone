#include "shell.h"
#include "pipe.h"

bool has_pipe(char* str) {
    char* seg_ptr = str;
    ParserState state = STATE_PARSER_NORMAL;
    while (*seg_ptr != '\0') {
        if (state == STATE_PARSER_NORMAL) {
            if (*seg_ptr == '\'') {
                state = STATE_IN_SINGLE_QUOTE;
            } else if (*seg_ptr == '"') {
                state = STATE_IN_DOUBLE_QUOTE;
            } else if (*seg_ptr == '\\') {
                seg_ptr++;
                if (*seg_ptr == '\0') break;
            } else if (*seg_ptr == PIPE) return true;
        } else if (state == STATE_IN_SINGLE_QUOTE) {
            if (*seg_ptr == '\'') state = STATE_PARSER_NORMAL;
        } else if (state == STATE_IN_DOUBLE_QUOTE) {
            if (*seg_ptr == '"') state = STATE_PARSER_NORMAL;
        }
        seg_ptr++;
    }
    return false;
}

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

    char* seg_ptr = line;
    bool in_seg = false; // Track if we're currently in a pipe segment
    ParserState state = STATE_PARSER_NORMAL;
    while (*seg_ptr != '\0') {
        if (state == STATE_PARSER_NORMAL) {
            if (*seg_ptr == '\'') {
                state = STATE_IN_SINGLE_QUOTE;
            } else if (*seg_ptr == '"') {
                state = STATE_IN_DOUBLE_QUOTE;
            } else if (*seg_ptr == '\\') {
                seg_ptr++;
                if (*seg_ptr == '\0') break;
            } else if (*seg_ptr == PIPE) {
                if (in_seg) {
                    *seg_ptr = '\0';
                    in_seg = false;
                }
            } else {
                if (!in_seg) {
                    segments[position++] = seg_ptr;

                    if (position >= buffsize) {
                        buffsize += SEG_BUFF_SIZE;
                        char** temp = realloc(segments, buffsize * sizeof(char*));
                        if (temp == NULL) {
                            free(segments);
                            fprintf(stderr, "ash: Allocation error\n");
                            exit(EXIT_FAILURE);
                        }
                        segments = temp;
                    }

                    in_seg = true;
                }
            }
        } else if (state == STATE_IN_SINGLE_QUOTE) {
            if (*seg_ptr == '\'') state = STATE_PARSER_NORMAL;
        } else if (state == STATE_IN_DOUBLE_QUOTE) {
            if (*seg_ptr == '"') state = STATE_PARSER_NORMAL;
        }
        seg_ptr++;
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

        if (!is_last) {
            if (pipe(fd) == -1) {
                perror("ash");
                if (fdin != STDIN_FILENO)
                    close(fdin);
                return 1;
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            reset_handlers(); // Reset signal handlers to default in child process

            // set stdin from previous pipe
            if (fdin != STDIN_FILENO) {
                if (dup2(fdin, STDIN_FILENO) == -1) {
                    perror("ash");
                    exit(EXIT_FAILURE);
                }
                close(fdin);
            }

            // set stdout to next pipe
            if (!is_last) {
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    perror("ash");
                    exit(EXIT_FAILURE);
                }
                close(fd[0]);
                close(fd[1]);
            }

            // parse and handle redirection within this segment
            char** args = split_line(trim(segments[i]));
            int redirected_fd = -1;
            int saved_fd = handle_redirection(args, &redirected_fd);
            if (redirected_fd != -1 && saved_fd == -1) {
                perror("ash");
                exit(EXIT_FAILURE);
            }

            sh_execute(args);
            // No need for saved_fd restoration in child

            free(args);
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
