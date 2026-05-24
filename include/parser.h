#ifndef PARSER_H
#define PARSER_H

#define RL_BUFF_SIZE 1024
#define TOK_BUFF_SIZE 64
#define TOK_DELIM " \t\r\n\a"

char* read_line();
char** split_line(char* line);

#endif
