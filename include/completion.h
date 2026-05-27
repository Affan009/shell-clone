#ifndef COMPLETION_H
#define COMPLETION_H

void init_completion();
char* cmd_generator(const char* text, int state);
char** completion(const char* text);

#endif
