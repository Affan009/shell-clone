#include "shell.h"
#include "parser.h"

char* read_line() {
    int buffsize = RL_BUFF_SIZE, position = 0;
    char* buffer = malloc(buffsize * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "ash: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    char c;
    while (true) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position++] = c;
        }

        if (position >= buffsize) {
            buffsize += RL_BUFF_SIZE;
            char* temp = realloc(buffer, buffsize * sizeof(char));
            if (temp == NULL) {
                free(buffer);
                fprintf(stderr, "ash: Allocation error\n");
                exit(EXIT_FAILURE);
            }
            buffer = temp;
        }
    }
}

char** split_line(char* line) {
    int buffsize = TOK_BUFF_SIZE, position = 0;
    char** tokens = malloc(buffsize * sizeof(char*));
    char* token;

    if (tokens == NULL) {
        fprintf(stderr, "ash: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= buffsize) {
            buffsize += TOK_BUFF_SIZE;
            char** temp = realloc(tokens, buffsize * sizeof(char*));
            if (temp == NULL) {
                free(tokens);
                fprintf(stderr, "ash: Allocation error\n");
                exit(EXIT_FAILURE);
            }
            tokens = temp;
        }

        token = strtok(NULL, TOK_DELIM);
    }

    tokens[position] = NULL;

    return tokens;
}
