#ifndef SIGHANDLER_H
#define SIGHANDLER_H

extern sigjmp_buf env;
extern volatile sig_atomic_t jump_active;

void init_handlers();
void sigint_handler(int sig);
void reset_handlers();

#endif
