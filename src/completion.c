#include "shell.h"
#include "completion.h"

void init_completion() {
    rl_attempted_completion_function = completion;
}

static char* cmd_generator(const char* text, int state) {
    static int list_index;
    static size_t len;
    static int search_phase; // 0 for built-ins, 1 for PATH
    static DIR* dir = NULL;

    static char* path_copy = NULL;
    static char* path_token = NULL;


    if (state == 0) {
        list_index = 0;
        len = strlen(text);
        search_phase = 0;

        if (dir) {
            closedir(dir);
            dir = NULL;
        }

        if (path_copy) {
            free(path_copy);
            path_copy = NULL;
        }
    }

    if (search_phase == 0) {
        while (list_index < num_builtins()) {
            const char* name = builtins[list_index].name;
            list_index++;

            if (strncmp(name, text, len) == 0) {
                return strdup(name);
            }
        }
        search_phase = 1;

        const char* path = getenv("PATH");
        if (path == NULL) {
            return NULL;
        }
        path_copy = strdup(path);

        if ((path_token = strtok(path_copy, ":")) != NULL) {
            dir = opendir(path_token);
        }
    }

    if (search_phase == 1) {
        while (path_token != NULL) {
            if (dir == NULL) {
                if ((path_token = strtok(NULL, ":")) != NULL) {
                    dir = opendir(path_token);
                }
                continue;
            }

            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                if (strncmp(entry->d_name, text, len) == 0) {
                    char full_path[strlen(path_token) + strlen(entry->d_name) + 2];
                    sprintf(full_path, "%s/%s", path_token, entry->d_name);

                    if (access(full_path, X_OK) == 0) {
                        return strdup(entry->d_name);
                    }
                }
            }

            closedir(dir);
            dir = NULL;

            if ((path_token = strtok(NULL, ":")) != NULL) {
                dir = opendir(path_token);
            }
        }
    }

    if (path_copy) {
        free(path_copy);
        path_copy = NULL;
    }

    if (dir) {
        closedir(dir);
        dir = NULL;
    }

    return NULL;
}

char** completion(const char* text, int start, int end) {
    rl_attempted_completion_over = 1; // Don't use filename completion
    if (start == 0)
        return rl_completion_matches(text, cmd_generator);
    return NULL;
}
