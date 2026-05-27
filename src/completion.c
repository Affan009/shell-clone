#include "shell.h"
#include "completion.h"

void init_completion() {
    rl_attempted_completion_function = completion;
}
