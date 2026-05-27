#include "shell.h"
#include "completion.h"

void init_completion() {
    rl_attempted_completion_function = completion;
}

static char* cmd_generator(const char* text, int state) {
    static int list_index;
    static size_t len;
    static int search_phase; // 0 for builtins, 1 for PATH
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
            path_token = NULL;
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
        path_token = NULL;
    }

    if (dir) {
        closedir(dir);
        dir = NULL;
    }

    return NULL;
}

static char* file_generator(const char* text, int state) {
    static DIR* dir = NULL;
    static char prefix[PATH_MAX];
    static char dirpath[PATH_MAX];
    static size_t len;

    if (state == 0) {
        if (dir) {
            closedir(dir);
            dir = NULL;
        }

        const char* slash = strrchr(text, '/');
        if (slash) {
            strncpy(dirpath, text, slash - text + 1);
            dirpath[slash - text + 1] = '\0';
            strncpy(prefix, slash + 1, sizeof(prefix) - 1);
            dir = opendir(dirpath);
        } else {
            strcpy(dirpath, "");
            strncpy(prefix, text, sizeof(prefix) - 1);
            dir = opendir(".");
        }
        prefix[sizeof(prefix) - 1] = '\0';
        len = strlen(prefix);
        if (!dir) return NULL;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strncmp(entry->d_name, prefix, len) == 0) {
            char full_path[strlen(dirpath) + strlen(entry->d_name) + 2];
            sprintf(full_path, "%s%s", dirpath, entry->d_name);
            
            struct stat st;
            if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                strcat(full_path, "/");
                rl_completion_append_character = '\0';
            } else rl_completion_append_character = ' ';

            return strdup(full_path);
        }
    }

    closedir(dir);
    dir = NULL;
    return NULL;
}

char** completion(const char* text, int start, int end) {
    (void)end; // Unused parameter
    rl_attempted_completion_over = 1; // Don't use filename completion
    if (start == 0)
        return rl_completion_matches(text, cmd_generator);
    return rl_completion_matches(text, file_generator);
}
