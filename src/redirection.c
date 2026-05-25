#include "shell.h"
#include "redirection.h"

int handle_redirection(char** args, int* redirected_fd) {
    int i, open_flags;

    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0 || strcmp(args[i], "1>") == 0) {
            *redirected_fd = STDOUT_FILENO;
            open_flags = OVERWRITE_FLAGS;
            break;
        }
    }

    if (*redirected_fd != -1) {
        char* filename = args[i + 1];
        if (filename == NULL) {
            fprintf(stderr, "ash: No filename provided for redirection\n");
            return -1;
        }

        args[i] = NULL;

        int saved_fd = dup(*redirected_fd);
        if (saved_fd == -1) {
            perror("ash");
            return -1;
        }

        int fd = open(filename, open_flags, 0644);
        if (fd == -1) {
            close(saved_fd);
            perror("ash");
            return -1;
        }

        if (dup2(fd, *redirected_fd) == -1) {
            close(saved_fd);
            close(fd);
            perror("ash");
            return -1;
        }
        
        close(fd);
        return saved_fd;
    }

    return -1;
}
