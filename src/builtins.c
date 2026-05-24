#include "shell.h"
#include "builtins.h"

const Builtin builtins[] = {
    {"exit", sh_exit},
    {"echo", sh_echo},
    {"type", sh_type},
    {"pwd", sh_pwd},
    {"cd", sh_cd},
    {"help", sh_help}
};

// Helper function to get the number of built-in commands
int num_builtins() {
    return sizeof(builtins) / sizeof(Builtin);
}

int sh_exit(char** args) {
    return 0;
}

int sh_echo(char** args) {
    for (size_t i = 1; args[i] != NULL; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}

int sh_type(char** args) {
    if (args[1] == NULL) {
        return 1;
    } else if (is_builtin(args[1])) {
        printf("%s is a shell builtin\n", args[1]);
    } else {
        // PATH lookup for program
        const char* path = getenv("PATH");
        if (path == NULL) {
            fprintf(stderr, "ash: PATH not set\n");
            return 1;
        }

        char* path_copy = strdup(path);
        for (char* dir = strtok(path_copy, ":"); dir != NULL; dir = strtok(NULL, ":")) {
            char full_path[strlen(dir) + strlen(args[1]) + 2];
            sprintf(full_path, "%s/%s", dir, args[1]);

            if (access(full_path, X_OK) == 0) {
                printf("%s is %s\n", args[1], full_path);
                free(path_copy);
                return 1;
            }
        }
        printf("%s: not found\n", args[1]);
        free(path_copy);
    }
    return 1;
}

int sh_pwd(char** args) {
    char* path = getcwd(NULL, 0);

    if (path == NULL) {
        perror("ash");
    } else {
        printf("%s\n", path);
        free(path);
    }

    return 1;
}

int sh_cd(char** args) {
    const char* path = NULL;
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "ash: HOME not set\n");
            return 1;
        }
    } else {
        path = args[1];
    }

    if (chdir(path) != 0) {
        perror("ash");
    }
    return 1;
}

int sh_help(char** args) {
    printf("ash: a C-shell (nice)\n");
    printf("Built-in commands:\n");

    for (int i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtins[i].str);
    }
    return 1;
}

bool is_builtin(const char* command) {
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(command, builtins[i].str) == 0) {
            return true;
        }
    }
    return false;
}
