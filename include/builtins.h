#ifndef BUILTINS_H
#define BUILTINS_H

typedef int (*builtin_func)(char**);

typedef struct {
    const char* str;
    builtin_func func;
} Builtin;

int num_builtins();

int sh_exit(char** args);
int sh_echo(char** args);
int sh_type(char** args);
int sh_pwd(char** args);
int sh_cd(char** args);
int sh_help(char** args);
bool is_builtin(const char* command);

extern const Builtin builtins[];

#endif
