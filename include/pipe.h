#ifndef PIPE_H
#define PIPE_H

#define SEG_BUFF_SIZE 64
#define PIPE "|"

char* trim(char* str);
char** split_pipe(char* line);
int pipe_execute(char** segments);

#endif
