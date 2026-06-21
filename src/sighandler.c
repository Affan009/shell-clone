#include "shell.h"
#include "sighandler.h"

sigjmp_buf env;
volatile sig_atomic_t jump_active = 0;

void init_handlers() {
    struct sigaction sa;
    
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Restart interrupted system calls
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN; // Ignore SIGSTP
    sigaction(SIGTSTP, &sa, NULL);
}

void sigint_handler(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    if (!jump_active) return;
    siglongjmp(env, 42);
}

void reset_handlers() {
    struct sigaction sa;
    
    sa.sa_handler = SIG_DFL; // Default handler
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
}
