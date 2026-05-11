#ifndef PARSER_H
#define PARSER_H

#define TOK_DELIM " \t\r\n\a"
#define TOK_BUFF_SIZE 64

char* read_line();
char** split_line(char* line);

#endif
