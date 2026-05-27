#ifndef COMPLETION_H
#define COMPLETION_H

void init_completion();
char** completion(const char* text, int start, int end);

#endif
