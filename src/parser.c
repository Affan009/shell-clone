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

    if (tokens == NULL) {
        fprintf(stderr, "ash: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    char* read_ptr = line;
    char* write_ptr = read_ptr;
    bool in_arg = false;
    ParserState state = STATE_PARSER_NORMAL;
    while (*read_ptr != '\0') {
        if (state == STATE_PARSER_NORMAL) {
            if (*read_ptr == '\'') {
                if (!in_arg) {
                    tokens[position++] = write_ptr;

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
                    
                    in_arg = true;
                }
                state = STATE_IN_SINGLE_QUOTE;
            } else if (*read_ptr == '"') {
                if (!in_arg) {
                    tokens[position++] = write_ptr;

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

                    in_arg = true;
                }
                state = STATE_IN_DOUBLE_QUOTE;
            } else if (strchr(TOK_DELIM, *read_ptr)) {
                if (in_arg) {
                    *write_ptr++ = '\0';
                    in_arg = false;
                }
            } else {
                if (!in_arg) {
                    tokens[position++] = write_ptr;

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

                    in_arg = true;
                }
                if (*read_ptr == '\\') {
                    read_ptr++;
                    if (*read_ptr == '\0') {
                        break;
                    }
                }
                *write_ptr++ = *read_ptr;
            }
        } else if (state == STATE_IN_SINGLE_QUOTE) {
            if (*read_ptr == '\'') {
                state = STATE_PARSER_NORMAL;
            } else {
                *write_ptr++ = *read_ptr;
            }
        } else if (state == STATE_IN_DOUBLE_QUOTE) {
            if (*read_ptr == '"') {
                state = STATE_PARSER_NORMAL;
            } else {
                if (*read_ptr == '\\') {
                    read_ptr++;
                    if (*read_ptr == '\0') {
                        break;
                    }

                    if (!strchr(ESCAPE_CHARS, *read_ptr)) {
                        *write_ptr++ = '\\';
                    }
                }
                *write_ptr++ = *read_ptr;
            }
        }
        read_ptr++;
    }

    if (in_arg) {
        *write_ptr = '\0';
    }

    tokens[position] = NULL;

    return tokens;
}
