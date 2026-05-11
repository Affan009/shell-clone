#include "shell.h"
#include "lexer.h"

char* read_line() {
    char* buffer = NULL;
    size_t buffsize = 0;

    if (getline(&buffer, &buffsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        }
        else {
            perror("ash");
            exit(EXIT_FAILURE);
        }
    }

    return buffer;
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
