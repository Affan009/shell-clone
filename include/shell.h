#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"
#include "builtins.h"
#include "executor.h"

void sh_loop();

#endif
