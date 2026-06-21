#ifndef SHELL_H
#define SHELL_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

#include "parser.h"
#include "builtins.h"
#include "executor.h"
#include "redirection.h"
#include "completion.h"
#include "history.h"
#include "pipe.h"
#include "sighandler.h"

void sh_loop();

#endif
